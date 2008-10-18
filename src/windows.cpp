/*
 ** Copyright (©) 2003-2008 Teus Benschop.
 **  
 ** This program is free software; you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation; either version 3 of the License, or
 ** (at your option) any later version.
 **  
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **  
 ** You should have received a copy of the GNU General Public License
 ** along with this program; if not, write to the Free Software
 ** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 **  
 */

#include "windows.h"
#include "settings.h"

void window_display(GtkWidget * window, WindowID id, ustring data, bool startup)
// Does the bookkeeping needed to display the window.
// window: pointer to the GtkWindow.
// id: the id of the window.
// data: the data attached to the window.
// startup: whether windows are handled at program startup.
{
  // If there's no data, then the configuration file gets inconsistent. Therefore put something there.
  if (data.empty()) {
    data.append("none");
  }

  extern Settings * settings;

  // The parameters of all the windows.
  WindowData window_data(true);

  // The new window's position.
  GdkRectangle new_window_rectangle;
  new_window_rectangle.x = 0;
  new_window_rectangle.y = 0;
  new_window_rectangle.width = 0;
  new_window_rectangle.height = 0;

  // At this stage we take it that the windows shows at program startup.
  // We extract the position and size of the window from the general configuration.
  // It does not matter here whether the space for the window is already taken up by another window,
  // because the user wishes it to be so.
  for (unsigned int i = 0; i < window_data.widths.size(); i++) {
    if ((window_data.ids[i] == id) && (window_data.datas[i] == data) && startup) {
      new_window_rectangle.x = window_data.x_positions[i];
      new_window_rectangle.y = window_data.y_positions[i];
      new_window_rectangle.width = window_data.widths[i];
      new_window_rectangle.height = window_data.heights[i];
    }
  }

  // Reject null values.
  if ((new_window_rectangle.width == 0) || (new_window_rectangle.height == 0) || (new_window_rectangle.x == 0) || (new_window_rectangle.y == 0))
    startup = false;

  // When a new window needs to be allocated, there are a few steps to be taken.
  if (!startup) {

    // Step 1: The area rectangle where the window should fit in is defined, e.g. the tools area, or the text area. 
    GdkRectangle area_rectangle;
    area_rectangle.x = 0;
    area_rectangle.y = 0;
    area_rectangle.width = 0;
    area_rectangle.height = 0;
    extern Settings * settings;
    switch (id)
    {
      case widShowKeyterms:
      case widShowQuickReferences:
      {
        area_rectangle.x = settings->genconfig.tools_area_x_position_get();
        area_rectangle.y = settings->genconfig.tools_area_y_position_get();
        area_rectangle.width = settings->genconfig.tools_area_width_get();
        area_rectangle.height = settings->genconfig.tools_area_height_get();
        break;
      }
        /*
         int text_area_width = settings->genconfig.text_area_width_get();
         int text_area_height = settings->genconfig.text_area_height_get();
         int text_area_x = settings->genconfig.text_area_x_position_get();
         int text_area_y = settings->genconfig.text_area_y_position_get();
         int notes_area_width = settings->genconfig.notes_area_width_get();
         int notes_area_height = settings->genconfig.notes_area_height_get();
         int notes_area_x = settings->genconfig.notes_area_x_position_get();
         int notes_area_y = settings->genconfig.notes_area_y_position_get();
         */
    }

    // Step 2: A GdkRegion is made of that area.  
    GdkRegion * area_region = gdk_region_rectangle(&area_rectangle);

    // Step 3: GdkRegions are made of each of the open windows, and each region is subtracted from the area region.
    for (unsigned int i = 0; i < settings->session.open_windows.size(); i++) {
      GtkWindow * open_window = settings->session.open_windows[i];
      GdkRectangle rectangle;
      gtk_window_get_size(open_window, &rectangle.width, &rectangle.height);
      gtk_window_get_position(open_window, &rectangle.x, &rectangle.y);
      GdkRegion * region = gdk_region_rectangle(&rectangle);
      gdk_region_subtract(area_region, region);
      gdk_region_destroy(region);
    }

    // Step 4: The rectangles that the area region consists of are requested,
    // and the biggest suitable rectangle is chosen for the window.
    // A rectangle is considered suitable if it has at least 10% of the width, and 10% of the height of the area rectangle.
    GdkRectangle *gdk_rectangles= NULL;
    gint rectangle_count = 0;
    gdk_region_get_rectangles(area_region, &gdk_rectangles, &rectangle_count);
    for (int i = 0; i < rectangle_count; ++i) {
      GdkRectangle & rectangle = gdk_rectangles[i];
      if (rectangle.width >= (area_rectangle.width / 10)) {
        if (rectangle.height >= (area_rectangle.height / 10)) {
          if ((rectangle.width * rectangle.height) > (new_window_rectangle.width * new_window_rectangle.height)) {
            new_window_rectangle = rectangle;
          }
        }
      }
    }
    g_free(gdk_rectangles);

    // Step 5: The area region is destroyed.
    gdk_region_destroy(area_region);

    // Step 6: If no area big enough is found, then the window that takes most space in the area is chosen, 
    // the longest side is halved, and the new window is put in that freed area.
    if ((new_window_rectangle.width == 0) || (new_window_rectangle.height == 0)) {
      GtkWindow * biggest_window = NULL;
      int largest_intersection = 0;
      for (unsigned int i = 0; i < settings->session.open_windows.size(); i++) {
        GtkWindow * open_window = settings->session.open_windows[i];
        GdkRectangle window_rectangle;
        gtk_window_get_size(open_window, &window_rectangle.width, &window_rectangle.height);
        gtk_window_get_position(open_window, &window_rectangle.x, &window_rectangle.y);
        GdkRectangle intersection_rectangle;
        if (gdk_rectangle_intersect (&area_rectangle, &window_rectangle, &intersection_rectangle)) {
          int intersection = intersection_rectangle.width * intersection_rectangle.height;
          if (intersection > largest_intersection) {
            biggest_window = open_window;
            largest_intersection = intersection;
          }
        }
      }
      if (biggest_window) {
        GdkRectangle window_rectangle;
        gtk_window_get_size(biggest_window, &window_rectangle.width, &window_rectangle.height);
        gtk_window_get_position(biggest_window, &window_rectangle.x, &window_rectangle.y);
        new_window_rectangle = window_rectangle;
        if (window_rectangle.width > window_rectangle.height) {
          window_rectangle.width /= 2;
          new_window_rectangle.width /= 2;
          new_window_rectangle.x += window_rectangle.width;
        } else {
          window_rectangle.height /= 2;
          new_window_rectangle.height /= 2;
          new_window_rectangle.y += window_rectangle.height;
        }
        gtk_window_resize(GTK_WINDOW (biggest_window), window_rectangle.width, window_rectangle.height);
      }
    }
  }

  // Set the window's position if there's something to be set.
  if (new_window_rectangle.width && new_window_rectangle.height) {
    gtk_window_resize(GTK_WINDOW (window), new_window_rectangle.width, new_window_rectangle.height);
    gtk_window_move(GTK_WINDOW (window), new_window_rectangle.x, new_window_rectangle.y);
  }

  // Store the window's parameters as showing.
  if (!startup) {
    bool window_found = false;
    for (unsigned int i = 0; i < window_data.widths.size(); i++) {
      if ((window_data.ids[i] == id) && (window_data.datas[i] == data)) {
        window_data.x_positions[i] = new_window_rectangle.x;
        window_data.y_positions[i] = new_window_rectangle.y;
        window_data.widths[i] = new_window_rectangle.width;
        window_data.heights[i] = new_window_rectangle.height;
        window_data.shows[i] = true;
        window_found = true;
        break;
      }
    }

    if (!window_found) {
      window_data.x_positions.push_back(new_window_rectangle.x);
      window_data.y_positions.push_back(new_window_rectangle.y);
      window_data.widths.push_back(new_window_rectangle.width);
      window_data.heights.push_back(new_window_rectangle.height);
      window_data.ids.push_back(id);
      window_data.datas.push_back(data);
      window_data.shows.push_back(true);
    }
  }

  // Store a pointer to this window in the Session.
  settings->session.open_windows.push_back(GTK_WINDOW (window));
}

void window_delete(GtkWidget * window, WindowID id, ustring data)
// Does the bookkeeping needed for deleting a window.
// When a window closes, the sizes of other windows are not affected. 
// Thus if the same window is opened again, it will go in the same free space as it was in before.
{
  // If there's no data, then the configuration file gets inconsistent. Therefore put something there.
  if (data.empty()) {
    data.append("none");
  }

  // Window position.
  gint width, height, x, y;
  gtk_window_get_size(GTK_WINDOW(window), &width, &height);
  gtk_window_get_position(GTK_WINDOW(window), &x, &y);

  // Get the parameters of all the windows.
  WindowData window_data(true);

  // Clear the "showing" flag for the window.
  for (unsigned int i = 0; i < window_data.ids.size(); i++) {
    if ((id == window_data.ids[i]) && (data == window_data.datas[i])) {
      window_data.shows[i] = false;
    }
  }

  // Remove the pointer to this window from the Session.
  GtkWindow * current_window= GTK_WINDOW (window);
  extern Settings * settings;
  vector <GtkWindow *> old_windows = settings->session.open_windows;
  vector <GtkWindow *> new_windows;
  for (unsigned int i = 0; i < old_windows.size(); i++) {
    if (current_window != old_windows[i]) {
      new_windows.push_back(old_windows[i]);
    }
  }
  settings->session.open_windows = new_windows;
}

WindowData::WindowData(bool save_on_destroy) {
  // Save variable.
  my_save_on_destroy = save_on_destroy;

  // Get the window data.
  extern Settings * settings;
  widths = settings->genconfig.window_widths_get();
  heights = settings->genconfig.window_heights_get();
  x_positions = settings->genconfig.window_x_positions_get();
  y_positions = settings->genconfig.window_y_positions_get();
  ids = settings->genconfig.window_ids_get();
  datas = settings->genconfig.window_datas_get();
  shows = settings->genconfig.window_shows_get();

  // If the configuration file has been fiddled with, then it may occur that the data is not consistent.
  // Check on this.
  bool data_consistent = true;
  if (widths.size() != heights.size())
    data_consistent = false;
  if (heights.size() != x_positions.size())
    data_consistent = false;
  if (x_positions.size() != y_positions.size())
    data_consistent = false;
  if (y_positions.size() != ids.size())
    data_consistent = false;
  if (ids.size() != datas.size())
    data_consistent = false;
  if (datas.size() != shows.size())
    data_consistent = false;
  if (!data_consistent) {
    // Clear it all out.
    widths.clear();
    heights.clear();
    x_positions.clear();
    y_positions.clear();
    ids.clear();
    datas.clear();
    shows.clear();
  }

}

WindowData::~WindowData() {
  // Store the data.
  if (my_save_on_destroy) {
    extern Settings * settings;
    settings->genconfig.window_widths_set(widths);
    settings->genconfig.window_heights_set(heights);
    settings->genconfig.window_x_positions_set(x_positions);
    settings->genconfig.window_y_positions_set(y_positions);
    settings->genconfig.window_ids_set(ids);
    settings->genconfig.window_datas_set(datas);
    settings->genconfig.window_shows_set(shows);
  }
}

void WindowData::debug()
// Prints the state.
{
  cout << "WindowData object " << this << " state" << endl;
  for (unsigned int i = 0; i < widths.size(); i++) {
    cout << "window " << i << ", width " << widths[i] << ", height " << heights[i] << ", x " << x_positions[i] << ", y " << y_positions[i] << ", id " << ids[i] << ", data " << datas[i] << ", show " << shows[i] << endl;
  }
}
