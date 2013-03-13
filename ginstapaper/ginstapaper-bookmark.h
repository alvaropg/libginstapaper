/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; tab-width: 8 -*-  */
/*
 * libginstapaper - GObject Instapaper library
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

#ifndef __GINSTAPAPER_BOOKMARK_H__
#define __GINSTAPAPER_BOOKMARK_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define GINSTAPAPER_TYPE_BOOKMARK             (ginstapaper_bookmark_get_type ())
#define GINSTAPAPER_BOOKMARK(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GINSTAPAPER_TYPE_BOOKMARK, GInstapaperBookmark))
#define GINSTAPAPER_BOOKMARK_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GINSTAPAPER_TYPE_BOOKMARK, GInstapaperBookmarkClass))
#define GINSTAPAPER_IS_BOOKMARK(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GINSTAPAPER_TYPE_BOOKMARK))
#define GINSTAPAPER_IS_BOOKMARK_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GINSTAPAPER_TYPE_BOOKMARK))
#define GINSTAPAPER_BOOKMARK_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GINSTAPAPER_TYPE_BOOKMARK, GInstapaperBookmarkClass))

typedef struct _GInstapaperBookmark         GInstapaperBookmark;
typedef struct _GInstapaperBookmarkPrivate  GInstapaperBookmarkPrivate;
typedef struct _GInstapaperBookmarkClass    GInstapaperBookmarkClass;

struct _GInstapaperBookmark
{
        GObject parent_instance;

        GInstapaperBookmarkPrivate *priv;
};

struct _GInstapaperBookmarkClass
{
        GObjectClass parent_class;
};

GType        ginstapaper_bookmark_get_type (void) G_GNUC_CONST;
const gchar *ginstapaper_bookmark_get_title (GInstapaperBookmark *bookmark);
const gchar *ginstapaper_bookmark_get_description (GInstapaperBookmark *bookmark);

G_END_DECLS

#endif /* __GINSTAPAPER_BOOKMARK_H__ */
