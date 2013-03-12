/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-  */
/*
 * gtk-foobar
 * Copyright (C) 2013 Álvaro Peña <alvaropg@gmail.com>
 * 
gtk-foobar is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * gtk-foobar is distributed in the hope that it will be useful, but
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
	gchar *title;
	gchar *description;
};

G_DEFINE_TYPE (GInstapaperBookmark, ginstapaper_bookmark, G_TYPE_OBJECT);

static void
ginstapaper_bookmark_init (GInstapaperBookmark *self)
{
	self->priv = G_TYPE_INSTANCE_GET_PRIVATE (self, GINSTAPAPER_TYPE_BOOKMARK, GInstapaperBookmarkPrivate);
}

static void
ginstapaper_bookmark_finalize (GObject *object)
{
	GInstapaperBookmarkPrivate *priv = GINSTAPAPER_BOOKMARK (object)->priv;

	g_free (priv->title);

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
                /* TODO: Add setter for "type" property here */
                break;
        case PROP_BOOKMARK_ID:
                /* TODO: Add setter for "bookmark_id" property here */
                break;
        case PROP_URL:
                /* TODO: Add setter for "url" property here */
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
                /* TODO: Add setter for "time" property here */
                break;
        case PROP_STARRED:
                /* TODO: Add setter for "starred" property here */
                break;
        case PROP_PRIVATE_SOURCE:
                /* TODO: Add setter for "private_source" property here */
                break;
        case PROP_HASH:
                /* TODO: Add setter for "hash" property here */
                break;
        case PROP_PROGRESS:
                /* TODO: Add setter for "progress" property here */
                break;
        case PROP_PROGRESS_TIMESTAMP:
                /* TODO: Add setter for "progress_timestamp" property here */
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
                /* TODO: Add getter for "type" property here */
                break;
        case PROP_BOOKMARK_ID:
                /* TODO: Add getter for "bookmark_id" property here */
                break;
        case PROP_URL:
                /* TODO: Add getter for "url" property here */
                break;
        case PROP_TITLE:
		g_value_set_string (value, priv->title);
                break;
        case PROP_DESCRIPTION:
		g_value_set_string (value, priv->description);
                /* TODO: Add getter for "description" property here */
                break;
        case PROP_TIME:
                /* TODO: Add getter for "time" property here */
                break;
        case PROP_STARRED:
                /* TODO: Add getter for "starred" property here */
                break;
        case PROP_PRIVATE_SOURCE:
                /* TODO: Add getter for "private_source" property here */
                break;
        case PROP_HASH:
                /* TODO: Add getter for "hash" property here */
                break;
        case PROP_PROGRESS:
                /* TODO: Add getter for "progress" property here */
                break;
        case PROP_PROGRESS_TIMESTAMP:
                /* TODO: Add getter for "progress_timestamp" property here */
                break;
        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
                break;
        }
}

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
                                                            "progress_timestam",
                                                            "progress_timestamp",
                                                            0, /* TODO: Adjust minimum property value */
                                                            G_MAXUINT, /* TODO: Adjust maximum property value */
                                                            0,
                                                            G_PARAM_READABLE | G_PARAM_WRITABLE));
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
