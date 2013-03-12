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

#include "ginstapaper-proxy.h"

#include <rest/oauth-proxy-call.h>

typedef struct {
        OAuthProxyAuthCallback callback;
        gpointer user_data;
} AuthData;

static void ginstapaper_proxy_class_init (GInstapaperProxyClass *klass);
static void ginstapaper_proxy_init       (GInstapaperProxy *self);
static void ginstapaper_proxy_finalize   (GObject *object);

G_DEFINE_TYPE (GInstapaperProxy, ginstapaper_proxy, OAUTH_TYPE_PROXY)

static void
ginstapaper_proxy_class_init (GInstapaperProxyClass *klass)
{
        GObjectClass *object_class = G_OBJECT_CLASS (klass);
        object_class->finalize = ginstapaper_proxy_finalize;
}

static void
ginstapaper_proxy_init (GInstapaperProxy *self)
{
}

static void
ginstapaper_proxy_finalize (GObject *object)
{
        G_OBJECT_CLASS (ginstapaper_proxy_parent_class)->finalize (object);
}

static void
access_token_cb (RestProxyCall *call,
                 const GError  *error,
                 GObject       *weak_object,
                 gpointer       user_data)
{
        AuthData *data = user_data;
        OAuthProxy *proxy = NULL;
        GHashTable *response_headers;

        g_object_get (call, "proxy", &proxy, NULL);
        g_assert (proxy);

        g_debug ("Payload: %s\nStatus Code: %d\nStatus Message: %s\n",
                 rest_proxy_call_get_payload (call),
                 rest_proxy_call_get_status_code (call),
                 rest_proxy_call_get_status_message (call));

        response_headers = rest_proxy_call_get_response_headers (call);
        g_hash_table_foreach (response_headers, _headers_fe, NULL);

        if (!error) {
                oauth_proxy_call_parse_token_reponse (OAUTH_PROXY_CALL (call));
        }

        data->callback (proxy, error, weak_object, data->user_data);

        g_slice_free (AuthData, data);
        g_object_unref (call);
        g_object_unref (proxy);
}

gboolean
ginstapaper_proxy_access_token_async (GInstapaperProxy       *gi_proxy,
                                      gchar                  *username,
                                      gchar                  *password,
                                      OAuthProxyAuthCallback  callback,
                                      GObject                *weak_object,
                                      gpointer                user_data,
                                      GError                 **error)
{
        RestProxyCall *call;
        AuthData *data;

        call = rest_proxy_new_call (REST_PROXY (gi_proxy));

        rest_proxy_call_set_function (call, "oauth/access_token");
        rest_proxy_call_set_method (call, "POST");

        rest_proxy_call_add_param (call, "x_auth_username", username);
        rest_proxy_call_add_param (call, "x_auth_password", password);
        rest_proxy_call_add_param (call, "x_auth_mode", "client_auth");

        data = g_slice_new0 (AuthData);
        data->callback = callback;
        data->user_data = user_data;

        return rest_proxy_call_async (REST_PROXY_CALL (call), access_token_cb, weak_object, data, error);
}

GInstapaperProxy*
ginstapaper_proxy_new (const gchar *consumer_key, const gchar *consumer_secret)
{
        GInstapaperProxy *proxy;

        proxy = g_object_new (GINSTAPAPER_TYPE_PROXY,
                              "url-format", "https://www.instapaper.com/api/1/",
                              "consumer-key", consumer_key, 
                              "consumer-secret", consumer_secret,
                              "signature-method", HMAC_SHA1,
                              "binding-required", FALSE,
                              NULL);

        return proxy;
}
