/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; tab-width: 8 -*-  */
/*
 * libginstapaper - GObject library for Instapaper
 * Copyright (C) 2013 Álvaro Peña <alvaropg@gmail.com>
 *
 * This library is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libginstapaper is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * SECTION:ginstapaper-bookmarks-call
 * @short_description: The call object for retrieve bookmarks data from Instapaper
 * @title: GInstapaperBookmarksCall
 * @include: ginstapaper/ginstapaper-bookmarks-call.h
 *
 * This object provides a easy to use #RestProxyCall object with the common functions available in the
 * Instapaper API.
 */

#include <json-glib/json-glib.h>

#include "ginstapaper-bookmarks-call.h"
#include "ginstapaper-bookmark.h"

#define LIST_FUNCTION     "bookmarks/list"
#define GET_TEXT_FUNCTION "bookmarks/get_text"

static void ginstapaper_bookmarks_call_class_init (GInstapaperBookmarksCallClass *klass);
static void ginstapaper_bookmarks_call_init       (GInstapaperBookmarksCall *self);
static void ginstapaper_bookmarks_call_finalize   (GObject *self);

G_DEFINE_TYPE (GInstapaperBookmarksCall, ginstapaper_bookmarks_call, OAUTH_TYPE_PROXY_CALL);

typedef struct {
        GInstapaperBookmarksListCallback callback;
        gpointer user_data;
} ListData;

typedef struct {
        GInstapaperBookmarkTextCallback callback;
        GInstapaperBookmark *bookmark;
        gpointer user_data;
} TextData;

static void
ginstapaper_bookmarks_call_class_init (GInstapaperBookmarksCallClass *klass)
{
        GObjectClass *object_class = G_OBJECT_CLASS (klass);
        object_class->finalize = ginstapaper_bookmarks_call_finalize;
}

static void
ginstapaper_bookmarks_call_init (GInstapaperBookmarksCall *self)
{
        /* All the Instapaper API functions require the params via POST method */
        rest_proxy_call_set_method (REST_PROXY_CALL (self), "POST");
}

static void
ginstapaper_bookmarks_call_finalize (GObject *self)
{
        G_OBJECT_CLASS(ginstapaper_bookmarks_call_parent_class)->finalize (self);
}

GList *
parse_bookmarks_list (const gchar *payload)
{
        GList *bookmarks = NULL;
        JsonParser *parser;
        JsonNode *root_node;
        GError *error;

        parser = json_parser_new ();
        if (json_parser_load_from_data (parser, payload, -1, &error)) {
                root_node = json_parser_get_root (parser);
                if (JSON_NODE_HOLDS_ARRAY (root_node)) {
                        JsonArray *jarray;
                        int i = 0;

                        jarray = json_node_get_array (root_node);
                        for (i = 0; i < json_array_get_length (jarray); i++) {
                                JsonNode *node;

                                node = json_array_get_element (jarray, i);
                                if (JSON_NODE_HOLDS_OBJECT (node)) {
                                        JsonObject *jobject;

                                        jobject = json_node_get_object (node);
                                        if (g_strcmp0 ("bookmark", json_object_get_string_member (jobject, "type")) == 0) {
                                                GInstapaperBookmark *bookmark;

                                                bookmark = GINSTAPAPER_BOOKMARK (json_gobject_deserialize (GINSTAPAPER_TYPE_BOOKMARK, node));
                                                if (GINSTAPAPER_IS_BOOKMARK (bookmark)) {
                                                        bookmarks = g_list_append (bookmarks, bookmark);
                                                } else {
                                                        g_debug ("Type bookmark object can't be deserialize");
                                                }
                                        }
                                }
                        }
                } else {
                        g_debug ("Root node isn't an array");
                }
        } else {
                g_debug ("Can't parse payload");
        }

        return bookmarks;
}

static void
list_cb (RestProxyCall *call, const GError *error, GObject *weak_object, gpointer user_data)
{
        ListData *data = user_data;
        GList *bookmarks = NULL;

        g_debug ("Payload: %s\nStatus Code: %d\nStatus Message: %s\n",
                 rest_proxy_call_get_payload (call),
                 rest_proxy_call_get_status_code (call),
                 rest_proxy_call_get_status_message (call));

        if (!error) {
                const gchar *payload;

                payload = rest_proxy_call_get_payload (call);
                bookmarks = parse_bookmarks_list (payload);
        }

        data->callback (bookmarks, error, data->user_data);

        g_slice_free (ListData, data);
        g_object_unref (call);
}

static void
get_text_cb (RestProxyCall *call, const GError *error, GObject *weak_object, gpointer user_data)
{
        TextData *data = user_data;
        const gchar *text = NULL;

        g_debug ("Payload: %s\nStatus Code: %d\nStatus Message: %s\n",
                 rest_proxy_call_get_payload (call),
                 rest_proxy_call_get_status_code (call),
                 rest_proxy_call_get_status_message (call));

        if (!error) {
                text = rest_proxy_call_get_payload (call);
        }

        data->callback (data->bookmark, text, error, data->user_data);

        g_slice_free (TextData, data);
        g_object_unref (call);
}

/**
 * ginstapaper_bookmarks_call_list:
 * @bookmarks_call: The #GInstapaperBookmarksCall
 * @limit: Optional. A number between 1 and 500, default 25. The limit of bookmarks for retrieve
 * @folder_id: Optional. Possible values are unread (default), starred, archive, or a folder_id value from /api/1/folders/list.
 * @have: Optional. A concatenation of bookmark_id values that the client already has from the specified folder.
 * @error: a #GError, or %NULL
 *
 * Return value: #GList with the #GInstapaperBookmarks if the get bookmarks list query was successfully executed, or %NULL on
 * failure. On failure @error is set.
 */
GList*
ginstapaper_bookmarks_call_list (GInstapaperBookmarksCall *bookmarks_call, guint limit, gchar *folder_id, gchar *have, GError **error)
{
        RestProxyCall *call;
        gboolean result;
        GList *bookmarks = NULL;

        g_return_val_if_fail (GINSTAPAPER_IS_BOOKMARKS_CALL (bookmarks_call), FALSE);
        g_return_val_if_fail (limit > 0 && limit < 501, FALSE);

        call = REST_PROXY_CALL (bookmarks_call);
        rest_proxy_call_set_function (call, LIST_FUNCTION);
        rest_proxy_call_set_method (call, "GET");
        rest_proxy_call_add_param (call, "limit", g_strdup_printf ("%d", limit));
        if (folder_id) {
                rest_proxy_call_add_param (call, "foder_id", folder_id);
        }
        if (have) {
                rest_proxy_call_add_param (call, "have", have);
        }

        result = rest_proxy_call_sync (call, error);
        if (result) {
                bookmarks = parse_bookmarks_list (rest_proxy_call_get_payload (call));
        }

        return bookmarks;
}

/**
 * ginstapaper_bookmarks_call_list_async:
 * @bookmarks_call: The #GInstapaperBookmarksCall
 * @limit: Optional. A number between 1 and 500, default 25. The limit of bookmarks for retrieve
 * @folder_id: Optional. Possible values are unread (default), starred, archive, or a folder_id value from /api/1/folders/list.
 * @have: Optional. A concatenation of bookmark_id values that the client already has from the specified folder.
 * @callback: a #GInstapaperBookmarksListCallback to call when the bookmarks list was retrieved.
 * @user_data: data to pass to @callback
 * @error: a #GError, or %NULL
 *
 * Return value: %TRUE if the get bookmarks list query was successfully queued, or %FALSE on
 * failure. On failure @error is set.
 */
gboolean
ginstapaper_bookmarks_call_list_async (GInstapaperBookmarksCall *bookmarks_call, guint limit, gchar *folder_id, gchar *have, GInstapaperBookmarksListCallback callback, gpointer user_data, GError **error)
{
        RestProxyCall *call;
        ListData *data;

        g_return_val_if_fail (GINSTAPAPER_IS_BOOKMARKS_CALL (bookmarks_call), FALSE);
        g_return_val_if_fail (limit > 0 && limit < 501, FALSE);

        call = REST_PROXY_CALL (bookmarks_call);
        rest_proxy_call_set_function (call, LIST_FUNCTION);
        rest_proxy_call_add_param (call, "limit", g_strdup_printf ("%d", limit));
        if (folder_id) {
                rest_proxy_call_add_param (call, "foder_id", folder_id);
        }
        if (have) {
                rest_proxy_call_add_param (call, "have", have);
        }

        data = g_slice_new0 (ListData);
        data->callback = callback;
        data->user_data = user_data;

        return rest_proxy_call_async (call, list_cb, NULL, data, error);
}

/**
 * ginstapaper_bookmark_get_text:
 * @bookmarks_call: a #GInstapaperBookmarksCall
 * @bookmark: a #GInstapaperBookmark
 * @text: a #gchar where the processed text-view HTML will be placed
 * @error: a #GError in case something goes wrong, or %NULL
 *
 * Synchronously retrieve of a specified bookmark’s processed text-view HTML.
 *
 * Return value: a #gchar with the processed text-view HTML (the string must be freed with g_free),
 * or %NULL on failure. On failure, @error is set.
 */
gchar*
ginstapaper_bookmarks_call_get_text (GInstapaperBookmarksCall *bookmarks_call, GInstapaperBookmark *bookmark, GError **error)
{
        RestProxyCall *call;
        guint bookmark_id;
        gboolean result;
        gchar *text = NULL;

        g_return_val_if_fail (GINSTAPAPER_IS_BOOKMARKS_CALL (bookmarks_call), FALSE);
        g_return_val_if_fail (GINSTAPAPER_IS_BOOKMARK (bookmark), FALSE);

        call = REST_PROXY_CALL (bookmarks_call);
        rest_proxy_call_set_function (call, GET_TEXT_FUNCTION);
        g_object_get (bookmark, "bookmark_id", &bookmark_id, NULL);
        rest_proxy_call_add_param (call, "bookmark_id", g_strdup_printf ("%d", bookmark_id));

        result = rest_proxy_call_sync (call, error);
        if (result) {
                text = g_strdup (rest_proxy_call_get_payload (call));
        }

        return text;
}

/**
 * ginstapaper_bookmarks_call_get_text_async:
 * @bookmarks_call: The #GInstapaperBookmarksCall.
 * @callback: a #GInstapaperBookmarkTextCallback to call when the bookmark's processed text-view was retrieved.
 * @user_data: data to pass to @callback.
 * @error: a #GError, or %NULL
 *
 * Asynchronously retrieve a specified bookmark¡s processed text-view HTML.
 *
 * Return value: %TRUE if the get text query was successfully queued, or %FALSE on
 * failure. On failure @error is set.
 */
gboolean
ginstapaper_bookmarks_call_get_text_async (GInstapaperBookmarksCall *bookmarks_call, GInstapaperBookmark *bookmark, GInstapaperBookmarkTextCallback callback, gpointer user_data, GError **error)
{
        RestProxyCall *call;
        guint bookmark_id;
        TextData *data;

        g_return_val_if_fail (GINSTAPAPER_IS_BOOKMARKS_CALL (bookmarks_call), FALSE);
        g_return_val_if_fail (GINSTAPAPER_IS_BOOKMARK (bookmark), FALSE);

        call = REST_PROXY_CALL (bookmarks_call);
        rest_proxy_call_set_function (call, GET_TEXT_FUNCTION);
        g_object_get (bookmark, "bookmark_id", &bookmark_id, NULL);
        rest_proxy_call_add_param (call, "bookmark_id", g_strdup_printf ("%d", bookmark_id));

        data = g_slice_new0 (TextData);
        data->callback = callback;
        data->user_data = user_data;
        data->bookmark = bookmark;

        return rest_proxy_call_async (call, get_text_cb, NULL, data, error);
}

/**
 * ginstapaper_bookmarks_call_new:
 * @proxy: the #GInstapaperProxy to use as API client.
 *
 * Creates a new #GInstapaperBookmarksCall.
 *
 * Return value: a new #GInstapaperBookmarksCall, or %NULL; unref with g_object_unref()
 */
GInstapaperBookmarksCall*
ginstapaper_bookmarks_call_new (GInstapaperProxy *proxy)
{
        g_return_val_if_fail (GINSTAPAPER_IS_PROXY (proxy), NULL);

        return GINSTAPAPER_BOOKMARKS_CALL (g_object_new (GINSTAPAPER_TYPE_BOOKMARKS_CALL, "proxy", proxy, NULL));
}
