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

/**
 * SECTION:ginstapaper-proxy
 * @short_description: The proxy client for Instapaper API
 * @title: GInstapaperProxy
 *
 * The proxy client for Instapaper allow to connect with the
 * Instapaper API.
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

        g_object_get (call, "proxy", &proxy, NULL);
        g_assert (proxy);

        g_debug ("Payload: %s\nStatus Code: %d\nStatus Message: %s\n",
                 rest_proxy_call_get_payload (call),
                 rest_proxy_call_get_status_code (call),
                 rest_proxy_call_get_status_message (call));

        if (!error) {
                oauth_proxy_call_parse_token_reponse (OAUTH_PROXY_CALL (call));
        }

        data->callback (proxy, error, weak_object, data->user_data);

        g_slice_free (AuthData, data);
        g_object_unref (call);
        g_object_unref (proxy);
}

/**
 * ginstapaper_proxy_access_token:
 * @gi_proxy: The #GInstapaperProxy client you want to get the access token
 * @username: The user's username
 * @password: The user's password
 * @error: a #GError, or %NULL
 *
 * Get the access token synchronously with an Instapaper account.
 *
 * After call some query, you need to get an access token. If you have an access 
 * token and an access token private already you can skip this step and set it with 
 * ginstapaper_proxy_set_tokens().
 *
 * Returns: %TRUE if the request token query was successfully queued, or %FALSE on
 * failure. On failure @error is set.
 */
gboolean
ginstapaper_proxy_access_token (GInstapaperProxy  *gi_proxy,
                                const gchar       *username,
                                const gchar       *password,
                                GError           **error)
{
        RestProxyCall *call;

        call = rest_proxy_new_call (REST_PROXY (gi_proxy));

        rest_proxy_call_set_function (call, "oauth/access_token");
        rest_proxy_call_set_method (call, "POST");

        rest_proxy_call_add_param (call, "x_auth_username", username);
        rest_proxy_call_add_param (call, "x_auth_password", password);
        rest_proxy_call_add_param (call, "x_auth_mode", "client_auth");

        if (!rest_proxy_call_run (call, NULL, error)) {
                g_object_unref (call);
                return FALSE;
        }

        oauth_proxy_call_parse_token_reponse (OAUTH_PROXY_CALL (call));

        g_object_unref (call);

        return TRUE;
}

/**
 * ginstapaper_proxy_access_token_async:
 * @gi_proxy: The #GInstapaperProxy client you want to get the access token
 * @username: The user's username
 * @password: The user's password
 * @callback: a #OAuthProxyAuthCallback to call when the access token process is finished
 * @weak_object: The #GObject to weakly reference and tie the lifecycle too
 * @user_data: data to pass to @callback
 * @error: a #GError, or %NULL
 * 
 * Get the access token asynchronously with an Instapaper account following the xAuth
 * specificication
 *
 * After call some query, you need to get an access token. If you have an access 
 * token and an access token private already you can skip this step and set it with 
 * ginstapaper_proxy_set_tokens().
 *
 * Returns: %TRUE if the request token query was successfully queued, or %FALSE on
 * failure. On failure @error is set.
 */
gboolean
ginstapaper_proxy_access_token_async (GInstapaperProxy       *gi_proxy,
                                      const gchar            *username,
                                      const gchar            *password,
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

/**
 * ginstapaper_proxy_new:
 * @consumer_key: The consumer key given by Instapaper
 * @consumer_secret: The consumer secret string given by Instapaper
 *
 * Creates a new #GInstapaperProxy client.
 *
 * Returns: The newly created #GInstapaperProxy client, or %NULL; unref with g_object_unref().
 */
GInstapaperProxy*
ginstapaper_proxy_new (const gchar *consumer_key, const gchar *consumer_secret)
{
        GInstapaperProxy *proxy;

        g_return_val_if_fail (g_utf8_strlen (consumer_key, -1) > 0, NULL);
        g_return_val_if_fail (g_utf8_strlen (consumer_secret, -1) > 0, NULL);

        proxy = g_object_new (GINSTAPAPER_TYPE_PROXY,
                              "url-format", "https://www.instapaper.com/api/1/",
                              "consumer-key", consumer_key, 
                              "consumer-secret", consumer_secret,
                              "signature-method", HMAC_SHA1,
                              "binding-required", FALSE,
                              NULL);

        return proxy;
}
