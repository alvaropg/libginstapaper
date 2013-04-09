#include <glib.h>
#include <stdlib.h>
#include <rest/oauth-proxy.h>
#include <ginstapaper/ginstapaper.h>

#include "credentials.h"

static GMainLoop *loop;

static void
get_text_cb (GInstapaperBookmark *bookmark, const gchar *text, const GError *error, gpointer user_data)
{
        g_print ("Text:\n%s\n", text);

        g_main_loop_quit (loop);
}

static void
get_list_cb (GList *bookmarks, const GError *error, gpointer user_data)
{
        GInstapaperBookmarksCall *call;
        GInstapaperBookmark *book;
        gchar *title, *url;
        GError *nerror = NULL;

        book = GINSTAPAPER_BOOKMARK (bookmarks->data);

        g_object_get (G_OBJECT (book),
                      "title", &title,
                      "url", &url,
                      NULL);

        g_print ("Title: %s\n", title);
        g_print ("URL: %s\n", url);

        call = ginstapaper_bookmarks_call_new (GINSTAPAPER_PROXY (user_data));
        ginstapaper_bookmarks_call_get_text_async (call, book, get_text_cb, NULL, &nerror);

        g_free (title);
        g_free (url);
        g_object_unref (book);
}

static void
get_bookmark (GInstapaperProxy *proxy)
{
        GInstapaperBookmarksCall *call;
        GError *error = NULL;

        call = ginstapaper_bookmarks_call_new (proxy);
        ginstapaper_bookmarks_call_list_async (call, 1, NULL, NULL, get_list_cb, proxy, &error);
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

  get_bookmark (GINSTAPAPER_PROXY (proxy));
}

int
main (int argc, char **argv)
{
        GInstapaperProxy *proxy;
        GError *error = NULL;

        g_type_init ();

        g_timeout_add_seconds (20, on_timeout, NULL);

        loop = g_main_loop_new (NULL, TRUE);

        proxy = ginstapaper_proxy_new (GINSTAPAPER_TEST_CONSUMER_KEY, GINSTAPAPER_TEST_CONSUMER_SECRET);
        ginstapaper_proxy_access_token_async (proxy, GINSTAPAPER_TEST_USERNAME, GINSTAPAPER_TEST_PASSWORD, access_token_cb, NULL, NULL, &error);

        g_main_loop_run (loop);

        g_main_loop_unref (loop);
        g_object_unref (proxy);

        return 0;
}
