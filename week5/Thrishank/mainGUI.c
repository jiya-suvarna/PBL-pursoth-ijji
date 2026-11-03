#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PAGES 100
#define MAX_FRAMES 20

#define FIFO 1
#define LRU 2
#define OPTIMAL 3

// Statistics structure
typedef struct
{
    int hits;
    int faults;
} Statistics;

// GUI widgets structure
typedef struct
{
    GtkWidget *window;
    GtkWidget *page_entry;
    GtkWidget *frame_entry;
    GtkWidget *fifo_check;
    GtkWidget *lru_check;
    GtkWidget *optimal_check;
    GtkTextBuffer *result_buffer;
} AppWidgets;


// Find a page in memory frames
int searchPage(int frames[], int count, int page)
{
    for (int i = 0; i < count; i++)
    {
        if (frames[i] == page)
            return i;
    }

    return -1;
}


// Find an empty frame
int findEmptyFrame(int frames[], int count)
{
    for (int i = 0; i < count; i++)
    {
        if (frames[i] == -1)
            return i;
    }

    return -1;
}


// Find least recently used frame
int findLRU(int lastUsed[], int count)
{
    int lru = 0;

    for (int i = 1; i < count; i++)
    {
        if (lastUsed[i] < lastUsed[lru])
            lru = i;
    }

    return lru;
}


// Find optimal replacement frame
int findOptimal(int pages[], int frames[],
                int frameCount, int current, int totalPages)
{
    int farthest = -1;
    int replaceIndex = 0;

    for (int i = 0; i < frameCount; i++)
    {
        int nextUse = totalPages + 1;

        for (int j = current + 1; j < totalPages; j++)
        {
            if (frames[i] == pages[j])
            {
                nextUse = j;
                break;
            }
        }

        if (nextUse > farthest)
        {
            farthest = nextUse;
            replaceIndex = i;
        }
    }

    return replaceIndex;
}


// Run page replacement simulation
Statistics simulate(int pages[], int totalPages,
                    int frameCount, int algorithm,
                    GString *output)
{
    int frames[MAX_FRAMES];
    int lastUsed[MAX_FRAMES];

    int fifoPointer = 0;

    Statistics stats = {0, 0};

    // Initialize frames
    for (int i = 0; i < frameCount; i++)
    {
        frames[i] = -1;
        lastUsed[i] = -1;
    }

    const char *name;

    if (algorithm == FIFO)
        name = "FIFO";
    else if (algorithm == LRU)
        name = "LRU";
    else
        name = "OPTIMAL";

    g_string_append_printf(output,
        "\n\n========================================\n"
        "             %s ALGORITHM\n"
        "========================================\n\n", name);

    // Table heading
    g_string_append_printf(output, "%-8s", "Page");

    for (int i = 0; i < frameCount; i++)
    {
        char frameName[20];

        snprintf(frameName, sizeof(frameName),
                 "Frame%d", i + 1);

        g_string_append_printf(output, "%-9s", frameName);
    }

    g_string_append_printf(output, "%-10s\n", "Status");

    g_string_append(output,
        "------------------------------------------------\n");

    // Process page references
    for (int i = 0; i < totalPages; i++)
    {
        int page = pages[i];

        int position = searchPage(
            frames, frameCount, page);

        // Page HIT
        if (position != -1)
        {
            stats.hits++;
            lastUsed[position] = i;
        }

        // Page FAULT
        else
        {
            stats.faults++;

            int empty = findEmptyFrame(
                frames, frameCount);

            int replaceIndex;

            if (empty != -1)
            {
                replaceIndex = empty;
            }
            else if (algorithm == FIFO)
            {
                replaceIndex = fifoPointer;
            }
            else if (algorithm == LRU)
            {
                replaceIndex = findLRU(
                    lastUsed, frameCount);
            }
            else
            {
                replaceIndex = findOptimal(
                    pages, frames, frameCount,
                    i, totalPages);
            }

            frames[replaceIndex] = page;
            lastUsed[replaceIndex] = i;

            if (algorithm == FIFO)
            {
                fifoPointer =
                    (replaceIndex + 1) % frameCount;
            }
        }

        // Display frame contents
        g_string_append_printf(output, "%-8d", page);

        for (int j = 0; j < frameCount; j++)
        {
            if (frames[j] == -1)
                g_string_append_printf(output, "%-9s", "-");
            else
                g_string_append_printf(
                    output, "%-9d", frames[j]);
        }

        g_string_append_printf(output, "%-10s\n",
            position != -1 ? "HIT" : "FAULT");
    }

    // Calculate statistics
    float hitRatio =
        (float)stats.hits / totalPages * 100;

    float faultRatio =
        (float)stats.faults / totalPages * 100;

    g_string_append(output,
        "\n----------------------------------------\n");

    g_string_append_printf(output,
        "Total Pages     : %d\n"
        "Page Hits       : %d\n"
        "Page Faults     : %d\n"
        "Hit Ratio       : %.2f%%\n"
        "Fault Ratio     : %.2f%%\n",
        totalPages,
        stats.hits,
        stats.faults,
        hitRatio,
        faultRatio);

    return stats;
}


// Parse page reference string
int parsePages(const char *input,
               int pages[], int *count)
{
    char *copy = g_strdup(input);
    char *ptr = copy;

    *count = 0;

    while (*ptr)
    {
        // Skip spaces and commas
        while (g_ascii_isspace(*ptr) || *ptr == ',')
            ptr++;

        if (*ptr == '\0')
            break;

        // Only non-negative page numbers
        if (!g_ascii_isdigit(*ptr))
        {
            g_free(copy);
            return 0;
        }

        char *end;
        long value = strtol(ptr, &end, 10);

        if (end == ptr || value > 2147483647L)
        {
            g_free(copy);
            return 0;
        }

        if (*count >= MAX_PAGES)
        {
            g_free(copy);
            return 0;
        }

        pages[*count] = (int)value;
        (*count)++;

        ptr = end;

        if (*ptr != '\0' &&
            !g_ascii_isspace(*ptr) && *ptr != ',')
        {
            g_free(copy);
            return 0;
        }
    }

    g_free(copy);

    return *count > 0;
}


// Show error popup
void showError(GtkWidget *window, const char *message)
{
    GtkWidget *dialog = gtk_message_dialog_new(
        GTK_WINDOW(window),
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_ERROR,
        GTK_BUTTONS_OK,
        "%s", message);

    gtk_dialog_run(GTK_DIALOG(dialog));

    gtk_widget_destroy(dialog);
}


// Run simulation button callback
void runSimulation(GtkWidget *button, gpointer data)
{
    (void)button;

    AppWidgets *app = (AppWidgets *)data;

    int pages[MAX_PAGES];
    int totalPages;
    int frameCount;

    // Get page reference input
    const char *pageText =
        gtk_entry_get_text(GTK_ENTRY(app->page_entry));

    const char *frameText =
        gtk_entry_get_text(GTK_ENTRY(app->frame_entry));

    // Validate page reference string
    if (!parsePages(pageText, pages, &totalPages))
    {
        showError(app->window,
            "Enter valid page numbers separated by spaces or commas. Maximum 100 pages.");
        return;
    }

    // Validate frame count
    char *end;

    long framesValue = strtol(frameText, &end, 10);

    while (g_ascii_isspace(*end))
        end++;

    if (frameText[0] == '\0' ||
        *end != '\0' ||
        framesValue < 1 ||
        framesValue > MAX_FRAMES)
    {
        showError(app->window,
            "Enter a valid number of frames between 1 and 20.");
        return;
    }

    frameCount = (int)framesValue;

    // Read algorithm selections
    gboolean runFIFO =
        gtk_toggle_button_get_active(
            GTK_TOGGLE_BUTTON(app->fifo_check));

    gboolean runLRU =
        gtk_toggle_button_get_active(
            GTK_TOGGLE_BUTTON(app->lru_check));

    gboolean runOptimal =
        gtk_toggle_button_get_active(
            GTK_TOGGLE_BUTTON(app->optimal_check));

    // At least one algorithm must be selected
    if (!runFIFO && !runLRU && !runOptimal)
    {
        showError(app->window,
            "Please select at least one algorithm.");
        return;
    }

    // Prepare results
    GString *output = g_string_new("");

    g_string_append(output,
        "PAGE REPLACEMENT SIMULATION RESULTS\n"
        "========================================\n\n");

    g_string_append_printf(output,
        "Page Reference String : %s\n"
        "Number of Pages       : %d\n"
        "Number of Frames      : %d\n",
        pageText, totalPages, frameCount);

    Statistics fifoStats = {0, 0};
    Statistics lruStats = {0, 0};
    Statistics optimalStats = {0, 0};

    // Run selected algorithms
    if (runFIFO)
    {
        fifoStats = simulate(
            pages, totalPages, frameCount,
            FIFO, output);
    }

    if (runLRU)
    {
        lruStats = simulate(
            pages, totalPages, frameCount,
            LRU, output);
    }

    if (runOptimal)
    {
        optimalStats = simulate(
            pages, totalPages, frameCount,
            OPTIMAL, output);
    }

    // Final comparison
    g_string_append(output,
        "\n\n========================================\n"
        "             FINAL COMPARISON\n"
        "========================================\n\n");

    g_string_append_printf(output,
        "%-12s %-8s %-9s %-12s\n",
        "Algorithm", "Hits", "Faults", "Hit Ratio");

    g_string_append(output,
        "----------------------------------------\n");

    if (runFIFO)
    {
        g_string_append_printf(output,
            "%-12s %-8d %-9d %.2f%%\n",
            "FIFO",
            fifoStats.hits,
            fifoStats.faults,
            (float)fifoStats.hits / totalPages * 100);
    }

    if (runLRU)
    {
        g_string_append_printf(output,
            "%-12s %-8d %-9d %.2f%%\n",
            "LRU",
            lruStats.hits,
            lruStats.faults,
            (float)lruStats.hits / totalPages * 100);
    }

    if (runOptimal)
    {
        g_string_append_printf(output,
            "%-12s %-8d %-9d %.2f%%\n",
            "Optimal",
            optimalStats.hits,
            optimalStats.faults,
            (float)optimalStats.hits / totalPages * 100);
    }

    g_string_append(output,
        "\nSimulation completed successfully!\n");

    // Display results in GUI
    gtk_text_buffer_set_text(
        app->result_buffer,
        output->str,
        -1);

    g_string_free(output, TRUE);
}


// Main GUI application
int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    // CSS styling
    const char *css =
        "window { background-color: #f5f5f5; }"

        ".main-title {"
        " font-size: 25px;"
        " font-weight: bold;"
        " color: #303638;"
        "}"

        ".subtitle {"
        " color: #555b60;"
        " font-size: 13px;"
        "}"

        ".field-label {"
        " font-size: 13px;"
        " color: #343a40;"
        "}"

        "entry {"
        " background-color: white;"
        " border: 1px solid #d4d4d4;"
        " border-radius: 5px;"
        " padding: 9px;"
        " font-size: 13px;"
        "}"

        "button.run-button {"
        " background-color: #287ee0;"
        " color: white;"
        " border-radius: 5px;"
        " padding: 10px;"
        " font-size: 13px;"
        "}"

        "button.run-button:hover {"
        " background-color: #1769c2;"
        "}"

        "textview {"
        " background-color: white;"
        " color: #252525;"
        " font-family: monospace;"
        " font-size: 12px;"
        "}";

    GtkCssProvider *provider =
        gtk_css_provider_new();

    gtk_css_provider_load_from_data(
        provider, css, -1, NULL);

    gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    // Main application window
    GtkWidget *window = gtk_window_new(
        GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title(
        GTK_WINDOW(window),
        "Page Replacement Simulator");

    gtk_window_set_default_size(
        GTK_WINDOW(window), 850, 750);

    gtk_window_set_position(
        GTK_WINDOW(window),
        GTK_WIN_POS_CENTER);

    gtk_container_set_border_width(
        GTK_CONTAINER(window), 30);

    g_signal_connect(
        window, "destroy",
        G_CALLBACK(gtk_main_quit), NULL);

    // Main vertical layout
    GtkWidget *mainBox =
        gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);

    gtk_container_add(
        GTK_CONTAINER(window), mainBox);

    // Initialize application widgets
    AppWidgets app;

    // IMPORTANT: Store main window in app structure
    app.window = window;

    // Main heading
    GtkWidget *title = gtk_label_new(
        "PAGE REPLACEMENT SIMULATOR");

    gtk_widget_set_halign(
        title, GTK_ALIGN_START);

    gtk_style_context_add_class(
        gtk_widget_get_style_context(title),
        "main-title");

    gtk_box_pack_start(
        GTK_BOX(mainBox),
        title, FALSE, FALSE, 0);

    // Subtitle
    GtkWidget *subtitle = gtk_label_new(
        "Operating Systems | Project-Based Learning");

    gtk_widget_set_halign(
        subtitle, GTK_ALIGN_START);

    gtk_style_context_add_class(
        gtk_widget_get_style_context(subtitle),
        "subtitle");

    gtk_box_pack_start(
        GTK_BOX(mainBox),
        subtitle, FALSE, FALSE, 5);

    // Page reference label
    GtkWidget *pageLabel = gtk_label_new(
        "Page-reference string");

    gtk_widget_set_halign(
        pageLabel, GTK_ALIGN_START);

    gtk_box_pack_start(
        GTK_BOX(mainBox),
        pageLabel, FALSE, FALSE, 0);

    // Page reference input
    app.page_entry = gtk_entry_new();

    gtk_entry_set_placeholder_text(
        GTK_ENTRY(app.page_entry),
        "Example: 7 0 1 2 0 3 0 4 2 3");

    gtk_box_pack_start(
        GTK_BOX(mainBox),
        app.page_entry, FALSE, FALSE, 0);

    // Frame count label
    GtkWidget *frameLabel = gtk_label_new(
        "Number of page frames");

    gtk_widget_set_halign(
        frameLabel, GTK_ALIGN_START);

    gtk_box_pack_start(
        GTK_BOX(mainBox),
        frameLabel, FALSE, FALSE, 0);

    // Frame count input
    app.frame_entry = gtk_entry_new();

    gtk_entry_set_placeholder_text(
        GTK_ENTRY(app.frame_entry),
        "Example: 3");

    gtk_box_pack_start(
        GTK_BOX(mainBox),
        app.frame_entry, FALSE, FALSE, 0);

    // Algorithm selection label
    GtkWidget *algorithmLabel = gtk_label_new(
        "Select algorithms (multiple selections allowed)");

    gtk_widget_set_halign(
        algorithmLabel, GTK_ALIGN_START);

    gtk_box_pack_start(
        GTK_BOX(mainBox),
        algorithmLabel, FALSE, FALSE, 0);

    // Algorithm checkboxes
    GtkWidget *algorithmBox =
        gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 25);

    app.fifo_check =
        gtk_check_button_new_with_label("FIFO");

    app.lru_check =
        gtk_check_button_new_with_label("LRU");

    app.optimal_check =
        gtk_check_button_new_with_label("Optimal");

    gtk_box_pack_start(
        GTK_BOX(algorithmBox),
        app.fifo_check, FALSE, FALSE, 0);

    gtk_box_pack_start(
        GTK_BOX(algorithmBox),
        app.lru_check, FALSE, FALSE, 0);

    gtk_box_pack_start(
        GTK_BOX(algorithmBox),
        app.optimal_check, FALSE, FALSE, 0);

    gtk_box_pack_start(
        GTK_BOX(mainBox),
        algorithmBox, FALSE, FALSE, 5);

    // Run simulation button
    GtkWidget *runButton =
        gtk_button_new_with_label("Run Simulation");

    gtk_style_context_add_class(
        gtk_widget_get_style_context(runButton),
        "run-button");

    gtk_box_pack_start(
        GTK_BOX(mainBox),
        runButton, FALSE, FALSE, 5);

    g_signal_connect(
        runButton,
        "clicked",
        G_CALLBACK(runSimulation),
        &app);

    // Results heading
    GtkWidget *resultLabel = gtk_label_new(
        "Simulation Results");

    gtk_widget_set_halign(
        resultLabel, GTK_ALIGN_START);

    gtk_box_pack_start(
        GTK_BOX(mainBox),
        resultLabel, FALSE, FALSE, 0);

    // Scrollable results area
    GtkWidget *scroll =
        gtk_scrolled_window_new(NULL, NULL);

    gtk_scrolled_window_set_policy(
        GTK_SCROLLED_WINDOW(scroll),
        GTK_POLICY_AUTOMATIC,
        GTK_POLICY_AUTOMATIC);

    gtk_widget_set_vexpand(scroll, TRUE);

    // Results text view
    GtkWidget *resultView = gtk_text_view_new();

    gtk_text_view_set_editable(
        GTK_TEXT_VIEW(resultView), FALSE);

    gtk_text_view_set_cursor_visible(
        GTK_TEXT_VIEW(resultView), FALSE);

    gtk_text_view_set_wrap_mode(
        GTK_TEXT_VIEW(resultView), GTK_WRAP_NONE);

    gtk_text_view_set_left_margin(
        GTK_TEXT_VIEW(resultView), 10);

    gtk_text_view_set_top_margin(
        GTK_TEXT_VIEW(resultView), 10);

    app.result_buffer =
        gtk_text_view_get_buffer(
            GTK_TEXT_VIEW(resultView));

    gtk_text_buffer_set_text(
        app.result_buffer,
        "Enter your inputs, select one or more algorithms, "
        "and click Run Simulation to get started.",
        -1);

    gtk_container_add(
        GTK_CONTAINER(scroll), resultView);

    gtk_box_pack_start(
        GTK_BOX(mainBox),
        scroll, TRUE, TRUE, 5);

    // Show application
    gtk_widget_show_all(window);

    gtk_main();

    // Release CSS provider
    g_object_unref(provider);

    return 0;
}
