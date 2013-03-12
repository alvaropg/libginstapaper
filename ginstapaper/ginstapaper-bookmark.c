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

#include "ginstapaper-bookmark.h"

enum
{
        PROP_0,

        PROP_TYPE,
        PROP_BOOKMARK_ID,
        PROP_URL,
        PROP_TITLE,
        PROP_DESCRIPTION,
        PROP_TIME,
        PROP_STARRED,
        PROP_PRIVATE_SOURCE,
        PROP_HASH,
        PROP_PROGRESS,
        PROP_PROGRESS_TIMESTAMP
};

struct _GInstapaperBookmarkPrivate {
	gchar    *type;
	guint     bookmark_id;
	gchar    *url;
	gchar    *title;
	gchar    *description;
	guint     time;
	gboolean  starred;
	gchar    *private_source;
	gchar    *hash;
	guint     progress;
	guint     progress_timestamp;
};

static void ginstapaper_bookmark_class_init   (GInstapaperBookmarkClass *klass);
static void ginstapaper_bookmark_init         (GInstapaperBookmark *self);
static void ginstapaper_bookmark_finalize     (GObject *object);
static void ginstapaper_bookmark_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec);
static void ginstapaper_bookmark_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec);

G_DEFINE_TYPE (GInstapaperBookmark, ginstapaper_bookmark, G_TYPE_OBJECT);

static void
ginstapaper_bookmark_class_init (GInstapaperBookmarkClass *klass)
{
        GObjectClass* object_class = G_OBJECT_CLASS (klass);
        GObjectClass* parent_class = G_OBJECT_CLASS (klass);

	g_type_class_add_private (klass, sizeof (GInstapaperBookmarkPrivate));

        object_class->finalize = ginstapaper_bookmark_finalize;
        object_class->set_property = ginstapaper_bookmark_set_property;
        object_class->get_property = ginstapaper_bookmark_get_property;

        g_object_class_install_property (object_class,
                                         PROP_TYPE,
                                         g_param_spec_string ("type",
                                                              "type",
                                                              "type",
                                                              "",
                                                              G_PARAM_READABLE | G_PARAM_WRITABLE));

        g_object_class_install_property (object_class,
                                         PROP_BOOKMARK_ID,
                                         g_param_spec_uint ("bookmark_id",
                                                            "bookmark_id",
                                                            "bookmark_id",
                                                            0, /* TODO: Adjust minimum property value */
                                                            G_MAXUINT, /* TODO: Adjust maximum property value */
                                                            0,
                                                            G_PARAM_READABLE | G_PARAM_WRITABLE));

        g_object_class_install_property (object_class,
                                         PROP_URL,
                                         g_param_spec_string ("url",
                                                              "url",
                                                              "url",
                                                              "",
                                                              G_PARAM_READABLE | G_PARAM_WRITABLE));

        g_object_class_install_property (object_class,
                                         PROP_TITLE,
                                         g_param_spec_string ("title",
                                                              "title",
                                                              "title",
                                                              "",
                                                              G_PARAM_READABLE | G_PARAM_WRITABLE));

        g_object_class_install_property (object_class,
                                         PROP_DESCRIPTION,
                                         g_param_spec_string ("description",
                                                              "description",
                                                              "description",
                                                              "",
                                                              G_PARAM_READABLE | G_PARAM_WRITABLE));

        g_object_class_install_property (object_class,
                                         PROP_TIME,
                                         g_param_spec_uint ("time",
                                                            "time",
                                                            "time",
                                                            0, /* TODO: Adjust minimum property value */
                                                            G_MAXUINT, /* TODO: Adjust maximum property value */
                                                            0,
                                                            G_PARAM_READABLE | G_PARAM_WRITABLE));

        g_object_class_install_property (object_class,
                                         PROP_STARRED,
                                         g_param_spec_string ("starred",
                                                              "starred",
                                                              "starred",
                                                              "",
                                                              G_PARAM_READABLE | G_PARAM_WRITABLE));

        g_object_class_install_property (object_class,
                                         PROP_PRIVATE_SOURCE,
                                         g_param_spec_string ("private_source",
                                                              "private_source",
                                                              "private_source",
                                                              "",
                                                              G_PARAM_READABLE | G_PARAM_WRITABLE));

        g_object_class_install_property (object_class,
                                         PROP_HASH,
                                         g_param_spec_string ("hash",
                                                              "hash",
                                                              "hash",
                                                              "",
                                                              G_PARAM_READABLE | G_PARAM_WRITABLE));

        g_object_class_install_property (object_class,
                                         PROP_PROGRESS,
                                         g_param_spec_uint ("progress",
                                                            "progress",
                                                            "progress",
                                                            0, /* TODO: Adjust minimum property value */
                                                            G_MAXUINT, /* TODO: Adjust maximum property value */
                                                            0,
                                                            G_PARAM_READABLE | G_PARAM_WRITABLE));

        g_object_class_install_property (object_class,
                                         PROP_PROGRESS_TIMESTAMP,
                                         g_param_spec_uint ("progress_timestamp",
                                                            "progress_timestamp",
                                                            "progress_timestamp",
                                                            0, /* TODO: Adjust minimum property value */
                                                            G_MAXUINT, /* TODO: Adjust maximum property value */
                                                            0,
                                                            G_PARAM_READABLE | G_PARAM_WRITABLE));
}

static void
ginstapaper_bookmark_init (GInstapaperBookmark *self)
{
	self->priv = G_TYPE_INSTANCE_GET_PRIVATE (self, GINSTAPAPER_TYPE_BOOKMARK, GInstapaperBookmarkPrivate);
}

static void
ginstapaper_bookmark_finalize (GObject *object)
{
	GInstapaperBookmarkPrivate *priv = GINSTAPAPER_BOOKMARK (object)->priv;

	g_free (priv->type);
	g_free (priv->url);
	g_free (priv->title);
	g_free (priv->description);
	g_free (priv->private_source);
	g_free (priv->hash);

        G_OBJECT_CLASS (ginstapaper_bookmark_parent_class)->finalize (object);
}

static void
ginstapaper_bookmark_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
	GInstapaperBookmarkPrivate *priv;

        g_return_if_fail (GINSTAPAPER_IS_BOOKMARK (object));
	priv = GINSTAPAPER_BOOKMARK (object)->priv;

        switch (prop_id)
        {
        case PROP_TYPE:
		if (priv->type)
			g_free (priv->type);
		priv->type = g_strdup (g_value_get_string (value));
                break;
        case PROP_BOOKMARK_ID:
		priv->bookmark_id = g_value_get_uint (value);
                break;
        case PROP_URL:
		if (priv->url)
			g_free (priv->url);
		priv->url = g_strdup (g_value_get_string (value));
                break;
        case PROP_TITLE:
                if (priv->title)
			g_free (priv->title);
		priv->title = g_strdup (g_value_get_string (value));
                break;
        case PROP_DESCRIPTION:
                if (priv->description)
			g_free (priv->description);
		priv->description = g_strdup (g_value_get_string (value));
                break;
        case PROP_TIME:
                priv->time = g_value_get_uint (value);
                break;
        case PROP_STARRED:
		if (g_strcmp0 (g_value_get_string (value), "1") == 0)
			priv->starred = TRUE;
		else
			priv->starred = FALSE;
                break;
        case PROP_PRIVATE_SOURCE:
		if (priv->private_source)
			g_free (priv->private_source);
		priv->private_source = g_strdup (g_value_get_string (value));
                break;
        case PROP_HASH:
		if (priv->hash)
			g_free (priv->hash);
		priv->hash = g_strdup (g_value_get_string (value));
                break;
        case PROP_PROGRESS:
                priv->progress = g_value_get_uint (value);
                break;
        case PROP_PROGRESS_TIMESTAMP:
                priv->progress_timestamp = g_value_get_uint (value);
                break;
        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
                break;
        }
}

static void
ginstapaper_bookmark_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
	GInstapaperBookmarkPrivate *priv;

        g_return_if_fail (GINSTAPAPER_IS_BOOKMARK (object));
	priv = GINSTAPAPER_BOOKMARK (object)->priv;

        switch (prop_id)
        {
        case PROP_TYPE:
		g_value_set_string (value, priv->type);
                break;
        case PROP_BOOKMARK_ID:
		g_value_set_uint (value, priv->bookmark_id);
                break;
        case PROP_URL:
		g_value_set_string (value, priv->url);
                break;
        case PROP_TITLE:
		g_value_set_string (value, priv->title);
                break;
        case PROP_DESCRIPTION:
		g_value_set_string (value, priv->description);
                break;
        case PROP_TIME:
		g_value_set_uint (value, priv->time);
                break;
        case PROP_STARRED:
                g_value_set_string (value, priv->starred ? "1" : "0");
                break;
        case PROP_PRIVATE_SOURCE:
		g_value_set_string (value, priv->private_source);
                break;
        case PROP_HASH:
		g_value_set_string (value, priv->hash);
                break;
        case PROP_PROGRESS:
                g_value_set_uint (value, priv->progress);
                break;
        case PROP_PROGRESS_TIMESTAMP:
		g_value_set_uint (value, priv->progress_timestamp);
                break;
        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
                break;
        }
}

const gchar*
ginstapaper_bookmark_get_title (GInstapaperBookmark *bookmark)
{
	g_return_val_if_fail (GINSTAPAPER_IS_BOOKMARK (bookmark), NULL);

	return bookmark->priv->title;
}

const gchar*
ginstapaper_bookmark_get_description (GInstapaperBookmark *bookmark)
{
	g_return_val_if_fail (GINSTAPAPER_IS_BOOKMARK (bookmark), NULL);

	return bookmark->priv->description;
}
