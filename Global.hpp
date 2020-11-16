/*
 * 4LN - Multiple Destinations Copier - batch copier to clone a content into several drives at once
 * Copyright (C) 2020 Martial Demolins AKA Folco
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * mail: martial <dot> demolins <at> gmail <dot> com
 */

#ifndef GLOBAL_HPP
#define GLOBAL_HPP

// Title of windows
#define WINDOW_TITLE "4LN - Multiple USB storages for TPMS"

// Minimum width of the progress windows (analyze and clone operations)
#define PROGRESS_WINDOW_MIN_WIDTH 900

// Minimum width of the main window
#define MAIN_WINDOW_MIN_WIDTH 640

// Minimum height of the main window
#define MAIN_WINDOW_MIN_HEIGHT 480

// Default overwrite size (kB)
#define OVERWRITE_SIZE 100

// Step for overwrite size (kB)
#define OVERWRITE_SIZE_STEP 100

// Overwrite settings
#define ORGANIZATION_NAME "FolcoSoft"
#define APPLICATION_NAME "4LN"
#define KEY_OVERWRITE "overwrite"
#define DEFAULT_OVERWRITE 1000

#endif // GLOBAL_HPP
