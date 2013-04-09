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

#ifndef __GINSTAPAPER_BOOKMARKS_CALL_H__
#define __GINSTAPAPER_BOOKMARKS_CALL_H__

#include <rest/oauth-proxy-call.h>
#include <ginstapaper/ginstapaper-proxy.h>
#include <ginstapaper/ginstapaper-bookmark.h>

G_BEGIN_DECLS

#define GINSTAPAPER_TYPE_BOOKMARKS_CALL             (ginstapaper_bookmarks_call_get_type())
#define GINSTAPAPER_BOOKMARKS_CALL(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj),GINSTAPAPER_TYPE_BOOKMARKS_CALL,GInstapaperBookmarksCall))
#define GINSTAPAPER_BOOKMARKS_CALL_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass),GINSTAPAPER_TYPE_BOOKMARKS_CALL,GInstapaperBookmarksCallClass))
#define GINSTAPAPER_IS_BOOKMARKS_CALL(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj),GINSTAPAPER_TYPE_BOOKMARKS_CALL))
#define GINSTAPAPER_IS_BOOKMARKS_CALL_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass),GINSTAPAPER_TYPE_BOOKMARKS_CALL))
#define GINSTAPAPER_BOOKMARKS_CALL_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj),GINSTAPAPER_TYPE_BOOKMARKS_CALL,GInstapaperBookmarksCallClass))

typedef struct _GInstapaperBookmarksCall      GInstapaperBookmarksCall;
typedef struct _GInstapaperBookmarksCallClass GInstapaperBookmarksCallClass;

struct _GInstapaperBookmarksCall {
        OAuthProxyCall parent;
};

struct _GInstapaperBookmarksCallClass {
        OAuthProxyCallClass parent_class;
};

/**
 * GInstapaperBookmarksListCallback:
 * @bookmarks: a #GList with #GInstapaperBookmark objects in
 * @error: a #GError
 * @user_data: user data passed to the callback
 *
 * Callback function called when the list function was finished.
 */
typedef void (*GInstapaperBookmarksListCallback)(GList *bookmarks, const GError *error, gpointer user_data);

/**
 * GInstapaperBookmarkTextCallback:
 * @bookmark: a #GInstapaperBookmark
 * @text: the bookmark's processed text-view HTML retrieved from Instapaper
 * @error: a #GError
 * @user_data: user data passed to the callback
 *
 * Callback function called when the processed text-view HTML from a bookmark was received.
 */
typedef void (*GInstapaperBookmarkTextCallback)(GInstapaperBookmark *bookmark, const gchar *text, const GError *error, gpointer user_data);

GType                     ginstapaper_bookmarks_call_get_type (void) G_GNUC_CONST;
GInstapaperBookmarksCall* ginstapaper_bookmarks_call_new      (GInstapaperProxy *proxy);

GList*                    ginstapaper_bookmarks_call_list       (GInstapaperBookmarksCall  *bookmarks_call,
                                                                 guint                      limit,
                                                                 gchar                     *folder_id,
                                                                 gchar                     *have,
                                                                 GError                    **error);
gboolean                  ginstapaper_bookmarks_call_list_async (GInstapaperBookmarksCall          *bookmarks_call,
                                                                 guint                              limit,
                                                                 gchar                             *folder_id,
                                                                 gchar                             *have,
                                                                 GInstapaperBookmarksListCallback   callback,
                                                                 gpointer                           user_data,
                                                                 GError                           **error);

gchar*                    ginstapaper_bookmarks_call_get_text       (GInstapaperBookmarksCall        *bookmarks_call,
                                                                     GInstapaperBookmark             *bookmark,
                                                                     GError                         **error);
gboolean                  ginstapaper_bookmarks_call_get_text_async (GInstapaperBookmarksCall        *bookmarks_call,
                                                                     GInstapaperBookmark             *bookmark,
                                                                     GInstapaperBookmarkTextCallback  callback,
                                                                     gpointer                         user_data,
                                                                     GError                         **error);

G_END_DECLS

#endif /* __GINSTAPAPER_BOOKMARKS_CALL_H__ */
