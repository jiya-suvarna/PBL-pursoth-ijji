#include <gtk/gtk.h>

static GtkWidget *page_entry;
static GtkWidget *frame_entry;
static GtkWidget *algorithm_dropdown;
static GtkWidget *status_label;

/* Run button callback */
static void on_run_clicked(GtkButton *button, gpointer user_data)
{
    const char *page_text;
    const char *frame_text;
    const char *algorithm;

    page_text = gtk_editable_get_text(
        GTK_EDITABLE(page_entry)
    );

    frame_text = gtk_editable_get_text(
        GTK_EDITABLE(frame_entry)
    );

    algorithm = gtk_string_list_get_string(
        GTK_STRING_LIST(
            gtk_drop_down_get_model(
                GTK_DROP_DOWN(algorithm_dropdown)
            )
        ),
        gtk_drop_down_get_selected(
            GTK_DROP_DOWN(algorithm_dropdown)
        )
    );

    if (page_text[0] == '\0' || frame_text[0] == '\0') {
        gtk_label_set_text(
            GTK_LABEL(status_label),
            "Please enter the page string and frame count."
        );
        return;
    }

    char *endptr;
    long frames = g_ascii_strtoll(frame_text, &endptr, 10);

    if (endptr == frame_text || *endptr != '\0' || frames <= 0) {
        gtk_label_set_text(
            GTK_LABEL(status_label),
            "Error: Enter a valid positive frame count."
        );
        return;
    }

    if (algorithm == NULL) {
        gtk_label_set_text(
            GTK_LABEL(status_label),
            "Please select an algorithm."
        );
        return;
    }

    gtk_label_set_text(
        GTK_LABEL(status_label),
        "Inputs accepted! Simulation integration comes next."
    );

    g_print("Page references: %s\n", page_text);
    g_print("Number of frames: %ld\n", frames);
    g_print("Selected algorithm: %s\n", algorithm);
}

/* Build the application window */
static void activate(GtkApplication *app, gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *main_box;
    GtkWidget *title_label;
    GtkWidget *subtitle_label;
    GtkWidget *page_label;
    GtkWidget *frame_label;
    GtkWidget *algorithm_label;
    GtkWidget *run_button;
    GtkWidget *card;

    const char *algorithms[] = {
        "FIFO",
        "LRU",
        "Optimal",
        "Compare All",
        NULL
    };

    window = gtk_application_window_new(app);

    gtk_window_set_title(
        GTK_WINDOW(window),
        "Page Replacement Simulator"
    );

    gtk_window_set_default_size(
        GTK_WINDOW(window),
        950,
        650
    );

    gtk_window_set_resizable(GTK_WINDOW(window), TRUE);

    main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 18);

    gtk_widget_set_margin_top(main_box, 28);
    gtk_widget_set_margin_bottom(main_box, 28);
    gtk_widget_set_margin_start(main_box, 32);
    gtk_widget_set_margin_end(main_box, 32);

    gtk_window_set_child(GTK_WINDOW(window), main_box);

    title_label = gtk_label_new(
        "PAGE REPLACEMENT SIMULATOR"
    );

    gtk_widget_add_css_class(title_label, "title-1");
    gtk_widget_set_halign(title_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(main_box), title_label);

    subtitle_label = gtk_label_new(
        "Operating Systems | Project-Based Learning"
    );

    gtk_widget_set_halign(subtitle_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(main_box), subtitle_label);

    card = gtk_box_new(GTK_ORIENTATION_VERTICAL, 14);

    gtk_widget_set_margin_top(card, 20);
    gtk_widget_set_margin_bottom(card, 10);

    gtk_box_append(GTK_BOX(main_box), card);

    page_label = gtk_label_new("Page-reference string");

    gtk_widget_set_halign(page_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(card), page_label);

    page_entry = gtk_entry_new();

    gtk_entry_set_placeholder_text(
        GTK_ENTRY(page_entry),
        "Example: 7 0 1 2 0 3 0 4 2 3"
    );

    gtk_box_append(GTK_BOX(card), page_entry);

    frame_label = gtk_label_new("Number of page frames");

    gtk_widget_set_halign(frame_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(card), frame_label);

    frame_entry = gtk_entry_new();

    gtk_entry_set_placeholder_text(
        GTK_ENTRY(frame_entry),
        "Example: 3"
    );

    gtk_box_append(GTK_BOX(card), frame_entry);

    algorithm_label = gtk_label_new("Select algorithm");

    gtk_widget_set_halign(algorithm_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(card), algorithm_label);

    algorithm_dropdown = gtk_drop_down_new_from_strings(
        algorithms
    );

    gtk_drop_down_set_selected(
        GTK_DROP_DOWN(algorithm_dropdown), 0
    );

    gtk_box_append(GTK_BOX(card), algorithm_dropdown);

    run_button = gtk_button_new_with_label(
        "Run Simulation"
    );

    gtk_widget_set_margin_top(run_button, 12);

    gtk_widget_add_css_class(run_button, "suggested-action");

    gtk_box_append(GTK_BOX(card), run_button);

    status_label = gtk_label_new(
        "Enter your inputs to get started."
    );

    gtk_label_set_wrap(GTK_LABEL(status_label), TRUE);
    gtk_widget_set_halign(status_label, GTK_ALIGN_START);

    gtk_box_append(GTK_BOX(main_box), status_label);

    g_signal_connect(
        run_button,
        "clicked",
        G_CALLBACK(on_run_clicked),
        NULL
    );

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv)
{
    GtkApplication *app;
    int status;

    app = gtk_application_new(
        "org.pbl.pagereplacement",
        G_APPLICATION_DEFAULT_FLAGS
    );

    g_signal_connect(
        app,
        "activate",
        G_CALLBACK(activate),
        NULL
    );

    status = g_application_run(
        G_APPLICATION(app),
        argc,
        argv
    );

    g_object_unref(app);

    return status;
}
