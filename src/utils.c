#include "utils.h"

// Validate song duration
bool is_valid_duration(const char *duration)
{
    int hours, minutes, seconds;
    if (sscanf(duration, "%2d:%2d:%2d", &hours, &minutes, &seconds) != 3)
        return false;
    if (hours < 0 || hours > 99 || minutes < 0 || minutes > 59 || seconds < 0 || seconds > 59)
        return false;
    if (strlen(duration) != 8 || duration[2] != ':' || duration[5] != ':')
    {
        return false;
    }
    return true;
}
// Validate user email
bool is_valid_email(const char *email)
{

    // Create copy of the email string
    char email_copy[256];
    strncpy(email_copy, email, sizeof(email_copy) - 1);
    email_copy[sizeof(email_copy) - 1] = '\0';

    // Remove quotes
    size_t len = strlen(email_copy);
    if (email_copy[0] == '"' && email_copy[len - 1] == '"')
    {
        memmove(email_copy, email_copy + 1, len - 2);
        email_copy[len - 2] = '\0';
        // printf("Stripped quotes: %s\n", email_copy);
    }

    // Step 1: Check for one '@' symbol
    const char *at = strchr(email_copy, '@');
    if (!at || strchr(at + 1, '@'))
    {
        // printf("Invalid: Email must contain a single '@' symbol\n");
        return false;
    }

    // Split to username and domain
    size_t username_len = at - email_copy;
    size_t domain_len = strlen(email_copy) - username_len - 1;

    char username[username_len + 1];
    char domain[domain_len + 1];

    strncpy(username, email_copy, username_len);
    username[username_len] = '\0';
    strncpy(domain, at + 1, domain_len);
    domain[domain_len] = '\0';

    // Step 2: Validate the username
    for (size_t i = 0; i < username_len; ++i)
    {
        if (!islower(username[i]) && !isdigit(username[i]))
        {
            // printf("Invalid: Username contains invalid character '%c'\n", username[i]);
            return false;
        }
    }

    // Step 3: Validate domain  <lstring>.<rstring>
    const char *dot = strchr(domain, '.');
    if (!dot || dot == domain || dot == domain + domain_len - 1)
    {
        // printf("Invalid: Domain must contain a dot in a valid position\n");
        return false;
    }

    // Split domain to lstring and rstring
    size_t lstring_len = dot - domain;
    size_t rstring_len = domain_len - lstring_len - 1;

    char lstring[lstring_len + 1];
    char rstring[rstring_len + 1];

    strncpy(lstring, domain, lstring_len);
    lstring[lstring_len] = '\0';
    strncpy(rstring, dot + 1, rstring_len);
    rstring[rstring_len] = '\0';

    // Validate lstring
    for (size_t i = 0; i < lstring_len; ++i)
    {
        if (!islower(lstring[i]))
        {
            // printf("Invalid: lstring contains invalid character '%c'\n", lstring[i]);
            return false;
        }
    }
    // Validate rstring
    if (rstring_len < 2 || rstring_len > 3)
    {
        // printf("Invalid: rstring length is %zu, which is outside the allowed range (2-3)\n", rstring_len);
        return false;
    }
    for (size_t i = 0; i < rstring_len; ++i)
    {
        if (!islower(rstring[i]))
        {
            // printf("Invalid: rstring contains invalid character '%c'\n", rstring[i]);
            return false;
        }
    }
    // printf("Email is valid\n");
    return true; // All validation checks passed
}

// Validate user subscription
bool is_valid_subscription(const char *type)
{
    return strcmp(type, "\"normal\"") == 0 || strcmp(type, "\"premium\"") == 0;
}

// Validate use birthdate
bool is_valid_date(Date date)
{
    if (date == NULL)
    {
        return false;
    }

    // Get year,month and day
    unsigned short year = get_year(date);
    unsigned char month = get_month(date);
    unsigned char day = get_day(date);

    if (year > 9999 || month < 1 || month > 12 || day < 1 || day > 31)
    {
        return false;
    }

    // Set current date
    const unsigned int current_year = 2024;
    const unsigned int current_month = 11;
    const unsigned int current_day = 30;

    if (year > current_year ||
        (year == current_year && month > current_month) ||
        (year == current_year && month == current_month && day > current_day))
    {
        return false;
    }

    return true;
}

void print_sec_to_time_format(unsigned int total_seconds)
{
    unsigned int hours = total_seconds / 3600;          // 1 hora = 3600 segundos
    unsigned int minutes = (total_seconds % 3600) / 60; // 1 minuto = 60 segundos
    unsigned int seconds = total_seconds % 60;          // segundos restantes

    printf("%02u:%02u:%02u", hours, minutes, seconds);
}

char *get_sec_to_time_format(unsigned int total_seconds)
{
    unsigned int hours = total_seconds / 3600;          // 1 hora = 3600 segundos
    unsigned int minutes = (total_seconds % 3600) / 60; // 1 minuto = 60 segundos
    unsigned int seconds = total_seconds % 60;          // segundos restantes

    if (hours > 99)
        hours = 99;

    // Aloca um buffer para a string de saída (hh:mm:ss + terminador nulo)
    char *time_str = malloc(9 * sizeof(char));
    if (time_str == NULL)
    {
        return NULL; // Retorna NULL em caso de erro de alocação
    }

    // Formata a string no buffer alocado
    sprintf(time_str, "%02u:%02u:%02u", hours, minutes, seconds);

    return time_str; // Retorna o ponteiro para a string alocada
}

char *remove_quotes(char *str)
{
    if (str == NULL || strlen(str) < 2)
    {
        return str; // Return the original if invalid or too short.
    }
    str++;                       // Move past the first quote.
    str[strlen(str) - 1] = '\0'; // Remove the trailing quote.
    return str;
}

LInt parse_id_list(char *string)
{
    LInt head = NULL;
    char *token;
    char *saveptr;

    string += 2; // avança o '"' e o '['

    token = strtok_r(string, ", ", &saveptr);
    while (token != NULL)
    {

        unsigned int id = strtoul(token + 2, NULL, 10); // avança o '\'' e o 'letracorrespondente'
                                                        // printf("Parsed ID: %u\n", id); // Debug print for each parsed ID
        lint_insert_head(&head, id);
        token = strtok_r(NULL, ", ", &saveptr);
    }

    return head;
}

Time parse_time(const char *duration_str)
{
    unsigned int hours, minutes, seconds;

    if (sscanf(duration_str, "%u:%u:%u", &hours, &minutes, &seconds) != 3)
    {
        return NULL; // Parsing failed
    }

    // Ensure values are within acceptable ranges
    if (hours > 99 || minutes > 59 || seconds > 59)
    {
        return NULL;
    }

    return create_time((unsigned char)hours, (unsigned char)minutes, (unsigned char)seconds);
}

Date parse_date(const char *birthdate_str)
{
    unsigned int year, month, day;
    // Check date is yyyy/mm/dd (length of 10 char)
    if (strlen(birthdate_str) != 10)
    {
        // printf("parse_date: Invalid date format length\n");
        return NULL;
    }
    // Check / position
    if (birthdate_str[4] != '/' || birthdate_str[7] != '/')
    {
        // printf("parse_date: Invalid date format - slashes in wrong positions\n");
        return NULL;
    }
    // Parse date
    if (sscanf(birthdate_str, "%4u/%2u/%2u", &year, &month, &day) != 3)
    {
        // printf("parse_date: Parsing failed. Expected format yyyy/mm/dd\n");
        return NULL;
    }
    // Validate ranges
    if (year > 9999 || month < 1 || month > 12 || day < 1 || day > 31)
    {
        // printf("parse_date: Out of range values - year: %u, month: %u, day: %u\n", year, month, day);
        return NULL;
    }
    // Create and return the date
    Date new_date = create_date((unsigned char)day, (unsigned char)month, (unsigned short)year);
    // printf("parse_date: Successfully created Date with year=%u, month=%u, day=%u\n", year, month, day);
    return new_date;
}

Date parse_user_date(const char *birth_date)
{
    // printf("parse_user_date: Original birth date string '%s'\n", birth_date);

    // Remove quotes
    char date_buffer[11];
    if (birth_date[0] == '"' && birth_date[strlen(birth_date) - 1] == '"')
    {
        strncpy(date_buffer, birth_date + 1, strlen(birth_date) - 2);
        date_buffer[strlen(birth_date) - 2] = '\0';
    }
    else
    {
        strncpy(date_buffer, birth_date, sizeof(date_buffer) - 1);
        date_buffer[sizeof(date_buffer) - 1] = '\0';
    }

    // printf("parse_user_date: Stripped birth date string '%s'\n", date_buffer);

    // Parse clean date
    Date DoB = parse_date(date_buffer);

    if (DoB != NULL)
    {
        // printf("parse_user_date: Successfully parsed Date. Day=%d, Month=%d, Year=%d\n",
        // get_day(DoB), get_month(DoB), get_year(DoB));

        // Validate the parsed Date
        if (is_valid_date(DoB))
        {
            // printf("parse_user_date: Date is valid.\n");
            return DoB;
        }
        else
        {
            // printf("parse_user_date: Date validation failed (e.g., future date).\n");
            free_date(DoB);
            return NULL;
        }
    }
    else
    {
        // printf("parse_user_date: Failed to parse Date from stripped string '%s'\n", date_buffer);
        return NULL;
    }
}

// Auxiliar para limpar o string da duration
void clean_string(char *str)
{
    int start = 0, end = strlen(str) - 1;

    // Trim leading spaces and quotes
    while (str[start] && (str[start] == '"' || isspace(str[start])))
    {
        start++;
    }

    // Trim trailing spaces and quotes
    while (end > start && (str[end] == '"' || isspace(str[end])))
    {
        end--;
    }

    // Shift the cleaned string to the start
    int i = 0;
    while (start <= end)
    {
        str[i++] = str[start++];
    }
    str[i] = '\0'; // Null-terminate the cleaned string
}

// Função para converter string no formato "HH:MM:SS" para segundos
int time_to_seconds(const char *time_str)
{
    int hours, minutes, seconds;
    if (sscanf(time_str, "%d:%d:%d", &hours, &minutes, &seconds) != 3)
    {
        fprintf(stderr, "Erro: formato de string inválido\n");
        return -1;
    }
    return hours * 3600 + minutes * 60 + seconds;
}

// Função para verificar se um ano é bissexto
bool is_leap_year(int year)
{
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
    {
        return true;
    }
    return false;
}

// Função para calcular o número de dias no mês
int days_in_month(int year, int month)
{
    switch (month)
    {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        return 31;
    case 4:
    case 6:
    case 9:
    case 11:
        return 30;
    case 2:
        return is_leap_year(year) ? 29 : 28;
    default:
        return 0;
    }
}

// Função para calcular o número total de dias desde a data de referência
int days_since_reference(int year, int month, int day)
{
    // Data de referência: 31 de dezembro de 2017
    int ref_year = 2017;
    int ref_month = 12;
    int ref_day = 31;

    int total_days = 0;

    // Caso em que o ano da data é maior que o ano de referência
    if (year > ref_year)
    {
        // Dias restantes no ano de referência
        total_days += days_in_month(ref_year, ref_month) - ref_day;
        for (int m = ref_month + 1; m <= 12; m++)
        {
            total_days += days_in_month(ref_year, m);
        }

        // Dias completos nos anos intermediários
        for (int y = ref_year + 1; y < year; y++)
        {
            total_days += is_leap_year(y) ? 366 : 365;
        }

        // Dias no ano da data
        for (int m = 1; m < month; m++)
        {
            total_days += days_in_month(year, m);
        }
        total_days += day;
    }
    else if (year < ref_year)
    {
        // Caso em que o ano da data é menor que o ano de referência
        // Dias restantes no ano da data
        total_days -= day;
        for (int m = month - 1; m >= 1; m--)
        {
            total_days -= days_in_month(year, m);
        }

        // Dias completos nos anos intermediários
        for (int y = year + 1; y < ref_year; y++)
        {
            total_days -= is_leap_year(y) ? 366 : 365;
        }

        // Dias no ano de referência
        for (int m = 12; m > ref_month; m--)
        {
            total_days -= days_in_month(ref_year, m);
        }
        total_days -= ref_day;
    }
    else
    {
        // Caso em que o ano é o mesmo
        if (month > ref_month)
        {
            for (int m = ref_month; m < month; m++)
            {
                total_days += days_in_month(year, m);
            }
            total_days += day - ref_day;
        }
        else if (month < ref_month)
        {
            for (int m = month; m < ref_month; m++)
            {
                total_days -= days_in_month(year, m);
            }
            total_days -= ref_day - day;
        }
        else
        {
            total_days = day - ref_day;
        }
    }

    return total_days;
}

// Função para calcular a semana a partir da data
int calculate_week(const char *date)
{
    int year, month, day;
    if (sscanf(date, "%d/%d/%d", &year, &month, &day) != 3)
    {
        fprintf(stderr, "Formato de data inválido. Use aaaa/mm/dd.\n");
        return -1;
    }

    int total_days = days_since_reference(year, month, day);

    // Cada semana tem 7 dias, começando em domingo e terminando no sábado
    int week_number = (total_days / 7) + 1;

    return week_number;
}

// Função para calcular o dia do ano
int day_of_year(int year, int month, int day)
{
    int day_of_year = 0;

    // Soma os dias dos meses anteriores
    for (int m = 1; m < month; m++)
    {
        day_of_year += days_in_month(year, m);
    }

    // Adiciona os dias do mês atual
    day_of_year += day;

    return day_of_year;
}

char *sec_to_string(int totalSeconds)
{
    char *timeString = (char *)malloc(9 * sizeof(char));
    if (timeString == NULL)
    {
        return NULL; // Falha na alocação
    }

    int hours = totalSeconds / 3600;
    int minutes = (totalSeconds % 3600) / 60;
    int seconds = totalSeconds % 60;

// Suprimir warnings para snprintf
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-truncation"
    snprintf(timeString, 9, "%02d:%02d:%02d", hours, minutes, seconds);
#pragma GCC diagnostic pop

    return timeString;
}

// Função para calcular a data a partir do ano e número de dias
char *calculate_date(int year, int days_passed)
{
    int month = 1;
    int day = 0;

    // Itera pelos meses para determinar o mês e o dia correspondente
    while (days_passed > days_in_month(year, month))
    {
        days_passed -= days_in_month(year, month);
        month++;
    }
    day = days_passed;

    // Aloca a string na heap e formata a data
    char *date = (char *)malloc(11 * sizeof(char));

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-truncation"
    snprintf(date, 11, "%04d/%02d/%02d", year, month, day);
#pragma GCC diagnostic pop

    return date;
}
