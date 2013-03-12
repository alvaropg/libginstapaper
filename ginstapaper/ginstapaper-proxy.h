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

#ifndef _GINSTAPAPER_PROXY
#define _GINSTAPAPER_PROXY

#include <rest/oauth-proxy.h>

G_BEGIN_DECLS

#define GINSTAPAPER_TYPE_PROXY            ginstapaper_proxy_get_type()
#define GINSTAPAPER_PROXY(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GINSTAPAPER_TYPE_PROXY, GInstapaperProxy))
#define GINSTAPAPER_PROXY_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GINSTAPAPER_TYPE_PROXY, GInstapaperProxyClass))
#define GINSTAPAPER_IS_PROXY(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GINSTAPAPER_TYPE_PROXY))
#define GINSTAPAPER_IS_PROXY_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GINSTAPAPER_TYPE_PROXY))
#define GINSTAPAPER_PROXY_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GINSTAPAPER_TYPE_PROXY, GInstapaperProxyClass))

typedef struct _GInstapaperProxy      GInstapaperProxy;
typedef struct _GInstapaperProxyClass GInstapaperProxyClass;

struct _GInstapaperProxy {
        OAuthProxy parent;
};

struct _GinstapaperProxyClass {
        OAuthProxyClass parent_class;
};

GType ginstapaper_proxy_get_type (void) G_GNUC_CONST;

GInstapaperProxy* ginstapaper_proxy_new                (const gchar            *consumer_key,
                                                        const gchar            *consumer_secret);
gboolean          ginstapaper_proxy_access_token_async (GInstapaperProxy       *gi_oauth,
                                                        gchar                  *username,
                                                        gchar                  *password,
                                                        OAuthProxyAuthCallback  callback,
                                                        GObject                *weak_object,
                                                        gpointer                user_data,
                                                        GError                 **error);

G_END_DECLS

#endif /* _GINSTAPAPER_PROXY */
