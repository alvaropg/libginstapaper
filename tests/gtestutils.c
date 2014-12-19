#include "config.h"
#include <glib.h>
#include <ginstapaper/ginstapaper.h>

#include "credentials.h"

GInstapaperProxy*
ginstapaper_test_setup (void)
{
        GInstapaperProxy *proxy;
        gboolean access_token_result;
        GError *error = NULL;

        proxy = ginstapaper_proxy_new (GINSTAPAPER_TEST_CONSUMER_KEY, GINSTAPAPER_TEST_CONSUMER_SECRET);
        g_assert (GINSTAPAPER_IS_PROXY (proxy));

        access_token_result = ginstapaper_proxy_access_token (proxy, GINSTAPAPER_TEST_USERNAME, GINSTAPAPER_TEST_PASSWORD, &error);
        g_assert_no_error (error);
        g_assert (access_token_result);

        return proxy;
}

static void
ginstapaper_test_bookmarks_list (GInstapaperProxy *proxy)
{
        GInstapaperBookmarksCall *call;
        GList *bookmarks = NULL;
        GError *error = NULL;

        call = ginstapaper_bookmarks_call_new (proxy);
        g_assert (GINSTAPAPER_IS_BOOKMARKS_CALL (call));

        bookmarks = ginstapaper_bookmarks_call_list (call, 4, NULL, NULL, &error);
        g_assert_no_error (error);
}

static void
ginstapaper_test_bookmarks_get_text (GInstapaperProxy *proxy)
{
        GInstapaperBookmarksCall *call;
        GList *bookmarks = NULL;
        GError *error = NULL;
        gchar *text;

        call = ginstapaper_bookmarks_call_new (proxy);
        g_assert (GINSTAPAPER_IS_BOOKMARKS_CALL (call));

        /* We just need one bookmark */
        bookmarks = ginstapaper_bookmarks_call_list (call, 1, NULL, NULL, &error);
        g_assert_no_error (error);
        if (g_list_length (bookmarks) == 0) {
                g_message ("The selected Instapaper user haven't any bookmark yet, so please, to test GInstapaper, add some bookmark");
                g_assert (FALSE);
        } else {
                error = NULL;
                call = ginstapaper_bookmarks_call_new (proxy);
                text = ginstapaper_bookmarks_call_get_text (call, GINSTAPAPER_BOOKMARK (bookmarks->data), &error);
                g_assert_no_error (error);
        }
}

int
main (int argc, char **argv)
{
        GInstapaperProxy *proxy;

        g_test_init (&argc, &argv, NULL);

        proxy = ginstapaper_test_setup ();

        g_test_add_data_func ("/GInstapaper/Bookmarks/List", proxy, (GTestDataFunc) ginstapaper_test_bookmarks_list);
        g_test_add_data_func ("/GInstapaper/Bookmarks/GetText", proxy, (GTestDataFunc) ginstapaper_test_bookmarks_get_text);

        return g_test_run ();
}
