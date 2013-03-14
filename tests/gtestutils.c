#include "config.h"
#include <glib.h>
#include <ginstapaper/ginstapaper.h>

#define GINSTAPAPER_TEST_CONSUMER_KEY    ""
#define GINSTAPAPER_TEST_CONSUMER_SECRET ""
#define GINSTAPAPER_TEST_USERNAME        ""
#define GINSTAPAPER_TEST_PASSWORD        ""

GInstapaperProxy*
ginstapaper_test_setup (void)
{
        GInstapaperProxy *proxy;
        gboolean access_token_result;

        proxy = ginstapaper_proxy_new (GINSTAPAPER_TEST_CONSUMER_KEY, GINSTAPAPER_TEST_CONSUMER_SECRET);
        g_assert (GINSTAPAPER_IS_PROXY (proxy));

        access_token_result = ginstapaper_proxy_access_token (proxy, GINSTAPAPER_TEST_USERNAME, GINSTAPAPER_TEST_PASSWORD, NULL);
        g_assert (access_token_result);

        return proxy;
}

static void
ginstapaper_test_bookmarks_list (GInstapaperProxy *proxy)
{
        GInstapaperBookmarksCall *call;
        GList *bookmarks = NULL;
        gboolean bookmarks_result;

        call = ginstapaper_bookmarks_call_new (proxy);
        g_assert (GINSTAPAPER_IS_BOOKMARKS_CALL (call));

        bookmarks_result = ginstapaper_bookmarks_call_list (call, 4, NULL, NULL, &bookmarks, NULL);
        g_assert (bookmarks_result);
}

int
main (int argc, char **argv)
{
        GInstapaperProxy *proxy;

        g_type_init ();
        g_test_init (&argc, &argv, NULL);

        proxy = ginstapaper_test_setup ();

        g_test_add_data_func ("/GInstapaper/Bookmarks/List", proxy, (GTestDataFunc) ginstapaper_test_bookmarks_list);

        return g_test_run ();
}
