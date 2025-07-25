#include <gtk/gtk.h>
#include "SortDisplay.h"

GtkWidget *sort_display;

// Enum to represent sorting algorithms
typedef enum
{
    BUBBLE_SORT,
    SELECTION_SORT,
    INSERTION_SORT,
    QUICK_SORT,
    MERGE_SORT,
    HEAP_SORT,
    SHELL_SORT,
    COUNTING_SORT,
    RADIX_SORT
} SortAlgorithm;

// Global variable to store the selected sorting algorithm
SortAlgorithm selected_algorithm = BUBBLE_SORT;

// Declare or include the sorting algorithm functions
gboolean bubble_sort(int *array, int *size);
gboolean selection_sort(int *array, int *size);
gboolean insertion_sort(int *array, int *size);
gboolean quick_sort(int *array, int *size);
gboolean merge_sort(int *array, int *size);
gboolean heap_sort(int *array, int *size);
gboolean shell_sort(int *array, int *size);
gboolean counting_sort(int *array, int *size);
gboolean radix_sort(int *array, int *size);

gboolean (*sort_algorithm_functions[])(int *, int *) = {
    bubble_sort,
    selection_sort,
    insertion_sort,
    quick_sort,
    merge_sort,
    heap_sort,
    shell_sort,
    counting_sort,
    radix_sort};

static int exec_count = 0;
static int bar_count = 0;
static gboolean first_run = TRUE;
static int delay_time = 100000; // 100 milliseconds

GMutex mutex; // Mutex for thread safety

// Callback function for radio button toggles
void on_sort_algorithm_changed(GtkToggleButton *button, gpointer user_data)
{
    if (gtk_toggle_button_get_active(button))
    {
        const gchar *label = gtk_button_get_label(GTK_BUTTON(button));
        if (g_strcmp0(label, "Bubble Sort") == 0)
            selected_algorithm = BUBBLE_SORT;
        else if (g_strcmp0(label, "Selection Sort") == 0)
            selected_algorithm = SELECTION_SORT;
        else if (g_strcmp0(label, "Insertion Sort") == 0)
            selected_algorithm = INSERTION_SORT;
        else if (g_strcmp0(label, "Quick Sort") == 0)
            selected_algorithm = QUICK_SORT;
        else if (g_strcmp0(label, "Merge Sort") == 0)
            selected_algorithm = MERGE_SORT;
        else if (g_strcmp0(label, "Heap Sort") == 0)
            selected_algorithm = HEAP_SORT;
        else if (g_strcmp0(label, "Shell Sort") == 0)
            selected_algorithm = SHELL_SORT;
        else if (g_strcmp0(label, "Counting Sort") == 0)
            selected_algorithm = COUNTING_SORT;
        else if (g_strcmp0(label, "Radix Sort") == 0)
            selected_algorithm = RADIX_SORT;
    }
}

// Timeout callback to update the bar chart periodically
gboolean update_bar_chart(gpointer user_data)
{
#ifdef TEST
    MySortDisplay *sort_display = MY_SORT_DISPLAY(user_data);
    if (sort_display)
    {
        shift_bar_chart_data(sort_display->data, sort_display->num_bars);
        g_main_context_invoke(NULL, (GSourceFunc)my_sort_display_update_surface, sort_display);
    }
#endif
    MySortDisplay *sortDisplay = MY_SORT_DISPLAY(user_data);
    g_mutex_lock(&mutex);
    g_main_context_invoke(NULL, (GSourceFunc)my_sort_display_update_surface, sort_display);
    g_mutex_unlock(&mutex);
    return TRUE;
}

gpointer update_bar_data(gpointer user_data)
{
    MySortDisplay *sortDisplay = MY_SORT_DISPLAY(user_data);
    sort_algorithm_functions[selected_algorithm](sortDisplay->data, &bar_count);
    return NULL; // Stop the timeout
}

// Function to generate random data and update the entry field
void generate_data(GtkButton *button, GtkEntry *entry)
{
    int random_data[16];
    char buffer[100] = {0};
    for (int i = 0; i < 16; i++)
    {
        random_data[i] = g_random_int_range(10, 100);
        char temp[16];
        snprintf(temp, sizeof(temp), "%d%s", random_data[i], (i < 16) ? "," : "");
        strcat(buffer, temp);
    }
    gtk_entry_set_text(entry, buffer);
}

void on_submit_clicked(GtkButton *button, GtkEntry *entry)
{
    const gchar *text = gtk_entry_get_text(entry);
    if (text && *text)
    {
        exec_count = 0; // 重置执行计数
        bar_count = 0;
        static int new_data[100];
        char *token = strtok((char *)text, ",");
        while (token && bar_count < 100)
        {
            new_data[bar_count++] = atoi(token);
            token = strtok(NULL, ",");
        }

        if (bar_count > 0)
        {
            my_sort_display_set_data(MY_SORT_DISPLAY(sort_display), new_data, bar_count);
        }
    }
}

gboolean update_recoder(GtkTextView *recodText)
{
    static int last_value = 0;
    if (exec_count == last_value)
    {
        return TRUE; // No update needed
    }
    GtkTextIter start_iter, end_iter;
    GtkTextMark *start_mark, *end_mark;

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(recodText));
    char message[50];

    snprintf(message, sizeof(message), "Record update step %d\n", exec_count);
    gtk_text_buffer_set_text(buffer, message, -1);

    return TRUE;
}

// Define the start_sorting function
void start_sorting(GtkButton *button, GtkTextView *recodText)
{
    // Example: Call the selected sorting algorithm
    // int data[] = {30, 70, 50, 90, 10};
    // int size = sizeof(data) / sizeof(data[0]);
    // sort_algorithm_functions[selected_algorithm](data, size);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(recodText));
    gtk_text_buffer_set_text(buffer, "Updating record...\n", -1);

    if (first_run)
    {
        first_run = FALSE;
        g_thread_new("", (GThreadFunc)update_bar_data, sort_display);
    }
}

int main(void)
{
    gtk_init(NULL, NULL);

    g_print("gtk version: %d.%d\n", gtk_get_major_version(), gtk_get_minor_version());

    g_mutex_init(&mutex);

    MY_TYPE_SORT_DISPLAY;

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 1200, 800);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_container_add(GTK_CONTAINER(window), hbox);

    GtkWidget *vbox_l = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GtkWidget *vbox_r = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    gtk_widget_set_size_request(vbox_l, 800, -1);
    gtk_widget_set_size_request(vbox_r, 400, -1);

    gtk_box_pack_start(GTK_BOX(hbox), vbox_l, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), vbox_r, TRUE, TRUE, 0);

    sort_display = my_sort_display_new();
    gtk_box_pack_start(GTK_BOX(vbox_l), sort_display, TRUE, TRUE, 0);

    GtkWidget *sortFrame = gtk_frame_new("Sort Algorithms");
    gtk_frame_set_label_align(GTK_FRAME(sortFrame), 0.5, 0.5);
    gtk_container_add(GTK_CONTAINER(vbox_r), sortFrame);
    gtk_frame_set_shadow_type(GTK_FRAME(sortFrame), GTK_SHADOW_IN);

    GtkWidget *sortBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_margin_top(GTK_WIDGET(sortBox), 10);
    gtk_container_add(GTK_CONTAINER(sortFrame), sortBox);

    GtkWidget *radio1 = gtk_radio_button_new_with_label(NULL, "Bubble Sort");
    GtkWidget *radio2 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radio1), "Selection Sort");
    GtkWidget *radio3 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radio1), "Insertion Sort");
    GtkWidget *radio4 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radio1), "Quick Sort");
    GtkWidget *radio5 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radio1), "Merge Sort");
    GtkWidget *radio6 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radio1), "Heap Sort");
    GtkWidget *radio7 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radio1), "Shell Sort");
    GtkWidget *radio8 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radio1), "Counting Sort");
    GtkWidget *radio9 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radio1), "Radix Sort");
    gtk_container_add(GTK_CONTAINER(sortBox), radio1);
    gtk_container_add(GTK_CONTAINER(sortBox), radio2);
    gtk_container_add(GTK_CONTAINER(sortBox), radio3);
    gtk_container_add(GTK_CONTAINER(sortBox), radio4);
    gtk_container_add(GTK_CONTAINER(sortBox), radio5);
    gtk_container_add(GTK_CONTAINER(sortBox), radio6);
    gtk_container_add(GTK_CONTAINER(sortBox), radio7);
    gtk_container_add(GTK_CONTAINER(sortBox), radio8);
    gtk_container_add(GTK_CONTAINER(sortBox), radio9);
    g_signal_connect(radio1, "toggled", G_CALLBACK(on_sort_algorithm_changed), NULL);
    g_signal_connect(radio2, "toggled", G_CALLBACK(on_sort_algorithm_changed), NULL);
    g_signal_connect(radio3, "toggled", G_CALLBACK(on_sort_algorithm_changed), NULL);
    g_signal_connect(radio4, "toggled", G_CALLBACK(on_sort_algorithm_changed), NULL);
    g_signal_connect(radio5, "toggled", G_CALLBACK(on_sort_algorithm_changed), NULL);
    g_signal_connect(radio6, "toggled", G_CALLBACK(on_sort_algorithm_changed), NULL);
    g_signal_connect(radio7, "toggled", G_CALLBACK(on_sort_algorithm_changed), NULL);
    g_signal_connect(radio8, "toggled", G_CALLBACK(on_sort_algorithm_changed), NULL);

    GtkWidget *numFrame = gtk_frame_new("Input Data");
    gtk_widget_set_margin_top(GTK_WIDGET(numFrame), 10);
    gtk_frame_set_label_align(GTK_FRAME(numFrame), 0.5, 0.5);

    GtkWidget *numBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    GtkWidget *numText = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(numText), "Enter numbers separated by commas (e.g., 30,70,50,90,10)");
    gtk_box_pack_start(GTK_BOX(numBox), numText, TRUE, TRUE, 0);

    GtkWidget *numButton = gtk_button_new_with_label("Radom Data");
    gtk_box_pack_end(GTK_BOX(numBox), numButton, FALSE, FALSE, 0);
    g_signal_connect(numButton, "clicked", G_CALLBACK(generate_data), numText);

    GtkWidget *submitButton = gtk_button_new_with_label("Submit");
    gtk_box_pack_end(GTK_BOX(numBox), submitButton, FALSE, FALSE, 0);
    g_signal_connect(submitButton, "clicked", G_CALLBACK(on_submit_clicked), numText);

    gtk_container_add(GTK_CONTAINER(numFrame), numBox);
    gtk_container_add(GTK_CONTAINER(vbox_r), numFrame);

    // Set the default sorting algorithm
    selected_algorithm = BUBBLE_SORT;

    GtkWidget *startFrame = gtk_frame_new("Start Sorting");
    gtk_frame_set_label_align(GTK_FRAME(startFrame), 0.5, 0.5);
    gtk_widget_set_margin_top(GTK_WIDGET(startFrame), 10);
    gtk_box_pack_start(GTK_BOX(vbox_r), startFrame, TRUE, TRUE, 0);

    GtkWidget *startBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_set_homogeneous(GTK_BOX(startBox), TRUE);

    GtkWidget *startButton = gtk_button_new_with_label("Start Sorting");
    gtk_box_pack_start(GTK_BOX(startBox), startButton, TRUE, TRUE, 0);

    GtkWidget *recodBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    GtkWidget *recodLabel = gtk_label_new("Sorting Record:");
    gtk_container_add(GTK_CONTAINER(recodBox), recodLabel);

    GtkWidget *recodText = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(recodText), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(recodText), FALSE);
    gtk_box_pack_start(GTK_BOX(recodBox), recodText, TRUE, TRUE, 0);

    gtk_container_add(GTK_CONTAINER(startBox), recodBox);
    gtk_container_add(GTK_CONTAINER(startFrame), startBox);

    g_signal_connect(startButton, "clicked", G_CALLBACK(start_sorting), recodText);

    g_timeout_add(100, (GSourceFunc)update_recoder, recodText);
    g_timeout_add(10, (GSourceFunc)update_bar_chart, sort_display);

    // Connect the "destroy" signal to exit the GTK main loop
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}

gboolean bubble_sort(int *array, int *size)
{
    if (*size < 1)
    {
        first_run = TRUE;
        exec_count = 0; // 重置执行计数
        return FALSE;   // 如果数组大小小于1，直接返回
    }
    // 遍历当前子数组 [0...n-2]
    // 注意这里是 n-1，因为我们要比较 arr[i] 和 arr[i+1]，所以 i 最大只能是 n-2
    for (int i = 0; i < *size - 1; ++i)
    {
        if (array[i] > array[i + 1])
        {
            // 如果前一个元素比后一个元素大，则交换它们
            int temp = array[i];
            array[i] = array[i + 1];
            array[i + 1] = temp;
            g_usleep(delay_time); // 延时以便观察排序过程
            exec_count++;
        }
    }

    *size -= 1; // 每次冒泡后，最大的元素已经在正确位置，所以可以减少数组大小

    bubble_sort(array, size); // 递归调用，继续冒泡排序

    return TRUE;
}

gboolean selection_sort(int *array, int *size)
{
    static int start_index = 0;

    if (start_index >= *size - 1)
    {
        start_index = 0; // 重置起始索引
        exec_count = 0;  // 重置执行计数
        first_run = TRUE;
        return FALSE; // 如果已经排序完成，直接返回
    }

    for (int i = start_index; i < *size; i++)
    {
        if (array[i] < array[start_index])
        {
            // 交换元素
            int temp = array[i];
            array[i] = array[start_index];
            array[start_index] = temp;
            g_usleep(delay_time); // 延时以便观察排序过程
            exec_count++;
        }
    }

    start_index++; // 增加起始索引，准备下一轮选择排序

    selection_sort(array, size); // 递归调用，继续选择排序
}

gboolean insertion_sort(int *array, int *size)
{
    static int current_index = 1;
    if (current_index >= *size)
    {
        current_index = 1; // 重置当前索引
        exec_count = 0;    // 重置执行计数
        first_run = TRUE;
        return FALSE; // 如果已经排序完成，直接返回
    }

    int current_value = array[current_index];
    int insert_position = 0;

    for (int i = current_index - 1; i > 0 || i == 0; i--)
    {
        if (array[i] > current_value)
        {
            // 将元素向后移动
            array[i + 1] = array[i];
            g_usleep(delay_time); // 延时以便观察排序过程
            exec_count++;
        }
        else
        {
            // 找到插入位置
            insert_position = i + 1;
            break;
        }
    }

    array[insert_position] = current_value; // 插入当前值到正确位置
    g_usleep(delay_time);                   // 延时以便观察排序过程

    current_index++; // 增加当前索引，准备下一轮插入排序

    insertion_sort(array, size); // 递归调用，继续插入排序
}

gboolean quick_sort(int *array, int *size)
{
    int pivot_pos = 0, left_pos = 0, right_pos = *size - 1;
    if (*size < 2)
    {
        first_run = TRUE;
        return FALSE; // 如果数组大小小于2，直接返回
    }

    // 选择第一个元素作为基准
    int pivot = array[pivot_pos];

    while (left_pos < right_pos)
    {
        while (left_pos < right_pos && array[right_pos] >= pivot)
        {
            right_pos--; // 从右向左找到第一个小于基准的元素
        }

        while (right_pos > left_pos && array[left_pos] <= pivot)
        {
            left_pos++; // 从左向右找到第一个大于基准的元素
        }

        if (left_pos < right_pos)
        {
            // 交换左右两边的元素
            int temp = array[left_pos];
            array[left_pos] = array[right_pos];
            array[right_pos] = temp;
            g_usleep(delay_time); // 延时以便观察排序过程
            exec_count++;
        }
    }

    // 将基准元素放到正确的位置
    if (left_pos != pivot_pos)
    {
        array[pivot_pos] = array[left_pos];
        array[left_pos] = pivot;
        g_usleep(delay_time); // 延时以便观察排序过程
        exec_count++;
    }

    // 递归调用对左边和右边的子数组进行快速排序
    int left_size = left_pos;                      // 左边子数组的大小
    int right_size = *size - left_pos - 1;         // 右边子数组的大小
    quick_sort(array, &left_size);                 // 对左边子数组进行快速排序
    quick_sort(array + left_pos + 1, &right_size); // 对右边子数组进行快速排序
}

void sub_merge(int *array, int size)
{
    if (size < 2)
    {
        first_run = TRUE;
        exec_count = 0; // 重置执行计数
        return;         // 如果数组大小小于2，直接返回
    }

    int mid = size / 2;

    // 递归分割数组
    sub_merge(array, mid);              // 对左半部分进行归并排序
    sub_merge(array + mid, size - mid); // 对右半部分进行归并排序

    // 合并两个已排序的子数组
    int *temp = g_malloc(size * sizeof(int)); // 临时数组用于存储合并结果
    int left_index = 0, right_index = mid, temp_index = 0;
    while (left_index < mid && right_index < size)
    {
        if (array[left_index] <= array[right_index])
        {
            temp[temp_index++] = array[left_index++];
        }
        else
        {
            temp[temp_index++] = array[right_index++];
        }
    }

    // 将剩余的左半部分元素添加到临时数组
    while (left_index < mid)
    {
        temp[temp_index++] = array[left_index++];
    }

    // 将剩余的右半部分元素添加到临时数组
    while (right_index < size)
    {
        temp[temp_index++] = array[right_index++];
    }

    // 将临时数组中的元素复制回原数组
    for (int i = 0; i < size; i++)
    {
        array[i] = temp[i];
        g_usleep(delay_time); // 延时以便观察排序过程
        exec_count++;         // 增加执行计数
    }

    g_free(temp); // 释放临时数组
}

gboolean merge_sort(int *array, int *size)
{
    sub_merge(array, *size);
    return TRUE; // 返回TRUE表示排序成功
}

gboolean heap_sort(int *array, int *size)
{
}

gboolean shell_sort(int *array, int *size)
{
}

gboolean counting_sort(int *array, int *size)
{
}

gboolean radix_sort(int *array, int *size)
{
}
