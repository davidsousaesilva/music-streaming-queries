#include "user.h"
#include "lint.h"
#include "utils.h"

struct user
{
    unsigned int username;
    char *email;
    char *first_name;
    char *last_name;
    Date birth_date;
    char *country;
    bool premium_acc; // false = normal | true = premium
    LInt liked_m_id;  // lista de id´s de musicas
    int age;          // Util para a query1 e query3
};

User create_user(unsigned int username, const char *email, const char *first_name,
                 const char *last_name, Date birth_date, const char *country,
                 bool premium_acc, LInt liked_m_id)
{
    // Alocar memória para o novo utilizador
    User new_user = malloc(sizeof(struct user));
    if (!new_user)
        return NULL; // Falha na alocação

    // Preencher os dados do novo utilizador
    new_user->username = username;
    new_user->email = strdup(email);
    new_user->first_name = strdup(first_name);
    new_user->last_name = strdup(last_name);
    new_user->birth_date = birth_date;
    new_user->country = strdup(country);
    new_user->premium_acc = premium_acc;
    new_user->liked_m_id = liked_m_id;

    return new_user;
}

User dup_user(User ori_user)
{
    User cpy_user = malloc(sizeof(struct user));
    if (!cpy_user)
        return NULL;

    cpy_user->username = ori_user->username;
    cpy_user->email = strdup(ori_user->email);
    cpy_user->first_name = strdup(ori_user->first_name);
    cpy_user->last_name = strdup(ori_user->last_name);
    cpy_user->birth_date = dup_date(ori_user->birth_date);
    cpy_user->country = strdup(ori_user->country);
    cpy_user->premium_acc = ori_user->premium_acc;
    cpy_user->liked_m_id = lint_dup_list(ori_user->liked_m_id);
    cpy_user->age = ori_user->age;

    return cpy_user;
}

void free_user(void *user_ptr)
{
    User user = (User)user_ptr;
    if (user)
    {
        free(user->email);
        free(user->first_name);
        free(user->last_name);
        free_date(user->birth_date);
        free(user->country);
        free_lint(user->liked_m_id);
        free(user);
    }
}

void print_user(void *user_ptr)
{
    User user = (User)user_ptr;
    if (user)
    {
        printf(" [Chave %d, Nome: %s, Idade: %d",
               user->username, user->first_name, user->age);
        print_date(user->birth_date);
        printf(", Lista: [");
        print_lint(user->liked_m_id);
        printf("] ->");
    }
}

char *get_email(User user)
{
    return strdup(user->email);
}

char *get_fst_name(User user)
{
    return strdup(user->first_name);
}

char *get_last_name(User user)
{
    return strdup(user->last_name);
}

char *get_country(User user)
{
    return strdup(user->country);
}

Date get_birth_date(User user)
{
    return user->birth_date;
}

int get_age(User user)
{
    return user->age;
}

LInt get_liked_songs(User user)
{
    return lint_dup_list(user->liked_m_id);
}

int calculate_age(Date birth_date)
{
    int current_year = 2024;
    int current_month = 9;
    int current_day = 9;

    int age = current_year - get_year(birth_date);
    if (get_month(birth_date) > current_month || (get_month(birth_date) == current_month && get_day(birth_date) > current_day))
    {
        age--;
    }
    return age;
}

void update_age_for_user(void *user_ptr, void *context)
{
    User user = (User)user_ptr;
    user->age = calculate_age(user->birth_date);
}

void parse_user(char *buffer, Users users, Songs catalog, FILE *error_file, char *sep)
{
    // printf("Processing user: %s\n", buffer);
    char *copyline = strdup(buffer);
    char *aux = buffer;

    // Parse CSV line
    char *username = strtok_r(aux, sep, &aux);
    char *email = strtok_r(NULL, sep, &aux);
    char *first_name = strtok_r(NULL, sep, &aux);
    char *last_name = strtok_r(NULL, sep, &aux);
    char *birth_date = strtok_r(NULL, sep, &aux);
    char *country = strtok_r(NULL, sep, &aux);
    char *subscription_type = strtok_r(NULL, sep, &aux);
    char *liked_musics_str = strtok_r(NULL, sep, &aux);

    bool valid = true;
    unsigned int user_id = strtoul(username + 2, NULL, 10);
    if (user_id == 0 && username[0] != '0')
    {
        // printf("Invalid username: %s\n", username);
        valid = false;
    }

    // Validate email
    if (!is_valid_email(email))
    {
        // printf("Invalid email: %s\n", email);
        valid = false;
    }

    LInt liked_musics = NULL;
    if (liked_musics_str[1] != '[')
    {
        valid = false;
    }
    else
    {
        // Validate Liked Musics
        liked_musics = parse_id_list(liked_musics_str);
    }

    LInt current = liked_musics;
    while (current != NULL)
    {
        unsigned int song_id = lint_get_value(current);
        if (!check_if_song_exists(catalog, song_id))
        {
            // printf("Song ID %u does not exist in the catalog.\n", song_id);
            valid = false;
        }
        // else{
        // printf("Song ID %u exists in the catalog.\n", song_id);}
        current = lint_get_next(current);
    }

    // Parse and Validate DoB
    Date DoB = parse_user_date(birth_date);
    if (DoB == NULL)
    {
        // printf("Invalid Date of Birth for user: %s\n", username);
        valid = false;
    }

    // Validate subscription
    if (!is_valid_subscription(subscription_type))
    {
        // printf("Invalid subscription type: %s\n", subscription_type);
        valid = false;
    } // else printf("Subscription type is: %s\n", subscription_type);

    // Insert user if valid, else log to error file
    if (valid)
    {
        // printf("User is valid. Inserting user: %s\n", username);
        User user = create_user(user_id, remove_quotes(email), remove_quotes(first_name), remove_quotes(last_name), DoB, remove_quotes(country), subscription_type, liked_musics);
        insert_user(users, user, user_id);
    }
    else
    {
        // printf("User is invalid. Logging to error file.\n");
        fprintf(error_file, "%s", copyline);
        free_lint(liked_musics);
        free_date(DoB);
    }

    free(copyline);
}
