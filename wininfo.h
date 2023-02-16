#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <algorithm>
#include <codecvt>
#include <cstring>
#include <locale>
#include <vector>

struct WindowInfo
{
    Window id;
    std::wstring title;
    std::vector<unsigned char> icon_data;
    int icon_width{0};
    int icon_height{0};
};

std::vector<WindowInfo> GetAllWindowInfo()
{
    std::vector<WindowInfo> window_infos;

    Display *display = XOpenDisplay(NULL);
    Window root_window = XDefaultRootWindow(display);

    Window parent_window;
    Window *child_windows;
    unsigned int num_child_windows;

    if (XQueryTree(display, root_window, &root_window, &parent_window,
                   &child_windows, &num_child_windows))
    {
        for (int i = 0; i < num_child_windows; i++)
        {
            WindowInfo window_info;
            window_info.id = child_windows[i];

            // Get window title
            char *window_name = nullptr;
            XTextProperty window_name_prop;
            if (XGetTextProperty(display, child_windows[i], &window_name_prop, XA_WM_NAME))
            {
                window_info.title = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes((const char *)window_name_prop.value);
                XFree(window_name_prop.value);
            }

            // Get window icon
            Atom actual_type;
            int actual_format;
            unsigned long nitems;
            unsigned long bytes_after;
            unsigned char *data;

            if (XGetWindowProperty(display, child_windows[i], XInternAtom(display, "_NET_WM_ICON", False),
                                   0, 1024, False, AnyPropertyType,
                                   &actual_type, &actual_format, &nitems, &bytes_after, &data) == Success)
            {
                if (nitems > 2)
                {
                    window_info.icon_width = data[0];
                    window_info.icon_height = data[1];
                    window_info.icon_data.resize(nitems - 2);
                    std::memcpy(window_info.icon_data.data(), data + 2, nitems - 2);
                }
                XFree(data);
            }

            window_infos.push_back(window_info);
        }
    }

    XFree(child_windows);
    XCloseDisplay(display);

    return window_infos;
}