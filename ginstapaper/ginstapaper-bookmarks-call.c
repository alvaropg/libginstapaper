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

#include <json-glib/json-glib.h>

#include "ginstapaper-bookmarks-call.h"
#include "ginstapaper-bookmark.h"

#define LIST_FUNCTION "bookmarks/list"

static void ginstapaper_bookmarks_call_class_init (GInstapaperBookmarksCallClass *klass);
static void ginstapaper_bookmarks_call_init       (GInstapaperBookmarksCall *self);
static void ginstapaper_bookmarks_call_finalize   (GObject *self);

G_DEFINE_TYPE (GInstapaperBookmarksCall, ginstapaper_bookmarks_call, OAUTH_TYPE_PROXY_CALL);

typedef struct {
        GInstapaperBookmarksListCallback callback;
        gpointer user_data;
} ListData;

static void
ginstapaper_bookmarks_call_class_init (GInstapaperBookmarksCallClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	object_class->finalize = ginstapaper_bookmarks_call_finalize;
}

static void
ginstapaper_bookmarks_call_init (GInstapaperBookmarksCall *self)
{
        rest_proxy_call_set_method (REST_PROXY_CALL (self), "POST");
}

static void
ginstapaper_bookmarks_call_finalize (GObject *self)
{
	G_OBJECT_CLASS(ginstapaper_bookmarks_call_parent_class)->finalize (self);
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
                JsonParser *parser;
                JsonNode *root_node;

                payload = rest_proxy_call_get_payload (call);
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
        }

        data->callback (bookmarks, error, data->user_data);
        
        g_slice_free (ListData, data);
        g_object_unref (call);
}

GInstapaperBookmarksCall*
ginstapaper_bookmarks_call_new (GInstapaperProxy *proxy)
{
        g_return_val_if_fail (GINSTAPAPER_IS_PROXY (proxy), NULL);

	return GINSTAPAPER_BOOKMARKS_CALL (g_object_new (GINSTAPAPER_TYPE_BOOKMARKS_CALL, "proxy", proxy, NULL));
}

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
