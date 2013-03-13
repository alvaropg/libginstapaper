#include <glib.h>
#include <stdlib.h>
#include <rest/oauth-proxy.h>
#include <ginstapaper/ginstapaper.h>

static GMainLoop *loop;

static void
get_list_cb (GList *bookmarks, const GError *error, gpointer user_data)
{
        while (bookmarks) {
                GInstapaperBookmark *book;
                gchar *title;

                book = bookmarks->data;
                g_object_get (G_OBJECT (book), "title", &title, NULL);
                g_print ("%s\n", title);
                g_free (title);
                g_object_unref (book);

                bookmarks = g_list_next (bookmarks);
        }

        g_main_loop_quit (loop);
}

static void
get_list (GInstapaperProxy *proxy)
{
        GInstapaperBookmarksCall *call;
        GError *error = NULL;

        call = ginstapaper_bookmarks_call_new (proxy);
        ginstapaper_bookmarks_call_list_async (call, 4, NULL, NULL, get_list_cb, NULL, &error);
}

static gboolean
on_timeout (gpointer data)
{
        g_message ("timed out!");
        exit (1);
        return FALSE;
}

static void
access_token_cb (OAuthProxy   *proxy,
                 const GError *error,
                 GObject      *weak_object,
                 gpointer      user_data)
{
  g_assert_no_error ((GError *)error);

  g_message ("Token: %s %s\n", oauth_proxy_get_token (proxy), oauth_proxy_get_token_secret (proxy));

  get_list (GINSTAPAPER_PROXY (proxy));
}

int
main (int argc, char **argv)
{
        GInstapaperProxy *proxy;
        GError *error = NULL;

        g_type_init ();

        g_timeout_add_seconds (20, on_timeout, NULL);

        loop = g_main_loop_new (NULL, TRUE);

        proxy = ginstapaper_proxy_new ("your_consumer_key", "your_consumer_secrete");
        ginstapaper_proxy_access_token_async (proxy, "username", "password", access_token_cb, NULL, NULL, &error);

        g_main_loop_run (loop);

        g_main_loop_unref (loop);
        g_object_unref (proxy);

        return 0;
}
