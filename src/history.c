#include "history_manager.h"

struct history
{
    unsigned int id;
    unsigned int user_id;
    unsigned int song_id;
    Date timestamp_date;
    Time timestamp_time;
    Time duration;
    bool mobile;
};

History create_history(unsigned int id, unsigned int user_id, unsigned int song_id, Date timestamp_date, Time timestamp_time, Time duration, bool mobile, Songs songs, Artists artists)
{

    History new_history = malloc(sizeof(struct history));
    if (new_history == NULL)
    {
        return NULL;
    }
    new_history->id = id;
    new_history->user_id = user_id;
    new_history->song_id = song_id;
    new_history->timestamp_date = timestamp_date;
    new_history->timestamp_time = timestamp_time;
    new_history->duration = duration;
    new_history->mobile = mobile;

    // Calcular a receita, cada history tem uma música associada
    // Vamos buscar esse musica
    Song song = get_song(song_id, songs);

    // Cada musica tem uma lista de artistas associada
    LInt author = get_author(song);

    // Vamos atualizar a receita da lista de autores associados a esta stream
    update_recipe(author, artists);

    free_lint(author);
    free_song(song);
    return new_history;
}

void free_history(void *history_ptr)
{
    History history = (History)history_ptr;
    if (history)
    {
        free_time(history->timestamp_time);
        free_date(history->timestamp_date);
        free_time(history->duration);
        free(history);
    }
}

void print_history(void *history_ptr)
{
    History history = (History)history_ptr;
    if (history)
    {
        printf(" [Chave %d, History ID: %u", history->id, history->user_id);
        printf(", Song ID: %u", history->song_id);
        printf(", Timestamp_Date: ");
        print_date(history->timestamp_date);

        printf(", Timestamp_Time: ");
        print_time(history->timestamp_time);

        printf(", Duration: ");
        print_time(history->duration);

        printf(", Mobile: %s]", history->mobile ? "Yes" : "No");
    }
}
// Partir o time stamp into date and time
void split_timestamp(const char *timestamp_str, char *date_str, char *time_str)
{
    char temp[30];
    size_t len = strlen(timestamp_str);

    // Remove surrounding quotes, if present
    if (timestamp_str[0] == '"' && timestamp_str[len - 1] == '"')
    {
        strncpy(temp, timestamp_str + 1, len - 2); // Copy without quotes
        temp[len - 2] = '\0';                      // Null-terminate
    }
    else
    {
        strncpy(temp, timestamp_str, sizeof(temp) - 1);
        temp[sizeof(temp) - 1] = '\0';
    }

    // Find space separator
    char *space_pos = strchr(temp, ' ');
    if (space_pos != NULL)
    {
        *space_pos = '\0';               // Replace space with null terminator
        strcpy(date_str, temp);          // Copy date part
        strcpy(time_str, space_pos + 1); // Copy  time part
    }
    else
    {
        // Handle invalid format
        date_str[0] = '\0';
        time_str[0] = '\0';
    }
}

void to_lowercase(char *str)
{
    for (int i = 0; str[i]; i++)
    {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void parse_history(char *buffer, Histories histories, FILE *error_file, char *sep, Songs songs, Artists artists, Stat_4 stat_4, Stat_5 stat_5)
{
    char *copyline = strdup(buffer); // Duplicate the line for safe parsing
    if (copyline == NULL)
    {
        fprintf(error_file, "Memory allocation failed for buffer: %s\n", buffer);
        return;
    }

    char *saveptr;

    // Tokenize the input buffer
    char *id_str = strtok_r(buffer, sep, &saveptr);
    char *user_id_str = strtok_r(NULL, sep, &saveptr);
    char *song_id_str = strtok_r(NULL, sep, &saveptr);
    char *timestamp_str = strtok_r(NULL, sep, &saveptr);
    char *duration_str = strtok_r(NULL, sep, &saveptr);
    char *platform_str = strtok_r(NULL, sep, &saveptr);
    bool valid = true;

    if (id_str == NULL || user_id_str == NULL || song_id_str == NULL ||
        timestamp_str == NULL || duration_str == NULL || platform_str == NULL)
    {
        fprintf(error_file, "%s", copyline);
        free(copyline);
        return;
    }

    // Parse IDs
    unsigned int id = strtoul(id_str + 2, NULL, 10);

    unsigned int user_id = strtoul(user_id_str + 2, NULL, 10);
    unsigned int song_id = strtoul(song_id_str + 2, NULL, 10);

    // Validate and parse timestamp
    char date_str[11] = {0}; // "YYYY/MM/DD" ( + null terminator)
    char time_str[9] = {0};  // "HH:MM:SS" (+ null terminator)
    split_timestamp(timestamp_str, date_str, time_str);

    if (date_str[0] == '\0' || time_str[0] == '\0')
    {
        fprintf(error_file, "Invalid timestamp format: %s\n", copyline);
        valid = false;
    }

    Date timestamp_date = NULL;
    Time timestamp_time = NULL;

    if (valid)
    {
        timestamp_date = parse_date(date_str);
        if (!timestamp_date || !is_valid_date(timestamp_date))
        {
            valid = false;
        }
        timestamp_time = parse_time(time_str);
        if (!timestamp_time)
        {
            valid = false;
        }
    }

    // Validate and parse duration
    clean_string(duration_str);
    Time duration = NULL;
    if (valid)
    {
        duration = parse_time(duration_str);
        if (!duration || !is_valid_duration(duration_str))
        {
            valid = false;
        }
    }

    // Validate platform
    bool mobile = false;
    clean_string(platform_str);
    if (strcasecmp(platform_str, "mobile") == 0)
    {
        mobile = true;
    }
    else if (strcasecmp(platform_str, "desktop") != 0)
    {
        valid = false;
    }

    // Create and insert history
    if (valid)
    {
        History new_history = create_history(id, user_id, song_id, timestamp_date, timestamp_time, duration, mobile, songs, artists);
        insert_history(histories, new_history, id);

        // stat 4
        insert_stat_4(songs, stat_4, song_id, date_str, duration_str);

        // stat 5
        Song song = get_song(song_id, songs);
        char *genre = get_genre(song);
        update_stat_5(stat_5, user_id, genre);
        free_song(song);
        free(genre);
    }
    else
    {
        fprintf(error_file, "%s", copyline);
    }

    // Cleanup
    free(copyline);
    if (!valid)
    {
        free_date(timestamp_date);
        free_time(timestamp_time);
        free_time(duration);
    }
}

void store_values_query_6(void *history_ptr, void *stat_6_ptr, void *songs_ptr)
{
    History history = (History)history_ptr;
    Songs songs = (Songs)songs_ptr;
    Stat_6 stat = (Stat_6)stat_6_ptr;

    // printf("%d\n", get_stat_6_id(stat));

    // Song song = get_song(history->song_id, songs);
    // LInt artists_list = get_author(song);
    // LInt current = artists_list;

    // while (current != NULL)
    //{
    if (history->user_id == get_stat_6_id(stat) && get_year(history->timestamp_date) == get_stat_6_year(stat))
    {
        add_data_stat_6(stat, songs, history->song_id, history->timestamp_date, history->timestamp_time, history->duration);
    }
    // current = lint_get_next(current);
    // }

    // free_song(song);
    // free_lint(artists_list);
}
