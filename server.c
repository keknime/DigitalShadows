#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <mysql.h>
#include <cjson/cJSON.h>

#define PORT 7777

void send_all(int sock, const char *buf, size_t len) {
    size_t sent = 0;
    while (sent < len) {
        ssize_t n = send(sock, buf + sent, len - sent, 0);
        if (n <= 0) break;
        sent += n;
    }
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in addr;
    char buf[1024];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) { perror("socket"); exit(1); }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);
    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind"); exit(1);
    }
    listen(server_fd, 5);

    MYSQL *conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, "host", "user", "pass", "db", 0, NULL, 0)) {
        fprintf(stderr, "mysql error: %s\n", mysql_error(conn));
        exit(1);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        socklen_t len = sizeof(addr);
        client_fd = accept(server_fd, (struct sockaddr*)&addr, &len);
        if (client_fd < 0) { perror("accept"); continue; }

        int n = recv(client_fd, buf, sizeof(buf)-1, 0);
        if (n <= 0) { close(client_fd); continue; }
        buf[n] = 0;

        char username[256], password[256];
        char *p = strchr(buf, '|');
        if (!p) {
            printf("DEBUG: Bad format\n");
            send_all(client_fd, "{\"error\":\"bad format\"}\n", 25);
            close(client_fd);
            continue;
        }
        *p = 0;
        strncpy(username, buf, 255); username[255] = 0;
        strncpy(password, p+1, 255); password[255] = 0;
        p = strchr(password, '\n');
        if (p) *p = 0;

        printf("DEBUG: Username: %s, Password: %s\n", username, password);

        MYSQL_RES *res;
        MYSQL_ROW row;
        char query[2048];
        snprintf(query, sizeof(query),
            "SELECT id FROM users WHERE username='%s' AND password='%s' LIMIT 1",
            username, password);

        if (mysql_query(conn, query) != 0) {
            printf("DEBUG: User lookup failed\n");
            send_all(client_fd, "{\"error\":\"db query failed\"}\n", 28);
            close(client_fd);
            continue;
        }
        res = mysql_store_result(conn);
        int user_id = -1;
        if ((row = mysql_fetch_row(res))) {
            user_id = atoi(row[0]);
            printf("DEBUG: user_id=%d\n", user_id);
        }
        mysql_free_result(res);

        if (user_id < 0) {
            printf("DEBUG: Login failed for user %s\n", username);
            send_all(client_fd, "{\"error\":\"login failed\"}\n", 27);
            close(client_fd);
            continue;
        }

        snprintf(query, sizeof(query),
            "SELECT * FROM characters WHERE user_id=%d", user_id);
        if (mysql_query(conn, query) != 0) {
            printf("DEBUG: Character lookup failed\n");
            send_all(client_fd, "{\"error\":\"db query failed\"}\n", 28);
            close(client_fd);
            continue;
        }
        res = mysql_store_result(conn);

        int num_fields = mysql_num_fields(res);
        MYSQL_FIELD *fields = mysql_fetch_fields(res);

        cJSON *out = cJSON_CreateArray();
        int char_found = 0;
        while ((row = mysql_fetch_row(res))) {
            cJSON *charobj = cJSON_CreateObject();
            for (int i = 0; i < num_fields; i++) {
                if (!row[i]) {
                    cJSON_AddNullToObject(charobj, fields[i].name);
                } else if (
                    fields[i].type == MYSQL_TYPE_TINY   || fields[i].type == MYSQL_TYPE_SHORT ||
                    fields[i].type == MYSQL_TYPE_LONG   || fields[i].type == MYSQL_TYPE_INT24 ||
                    fields[i].type == MYSQL_TYPE_LONGLONG || fields[i].type == MYSQL_TYPE_DECIMAL ||
                    fields[i].type == MYSQL_TYPE_NEWDECIMAL
                ) {
                    cJSON_AddNumberToObject(charobj, fields[i].name, atoi(row[i]));
                } else {
                    cJSON_AddStringToObject(charobj, fields[i].name, row[i]);
                }
            }

            int char_id = -1;
            for (int i = 0; i < num_fields; i++) {
                if (strcmp(fields[i].name, "id") == 0 && row[i]) {
                    char_id = atoi(row[i]);
                    printf("DEBUG: char_id=%d\n", char_id);
                    break;
                }
            }

            // Bag lookup
            if (char_id >= 0) {
                snprintf(query, sizeof(query), "SELECT * FROM character_inventory WHERE char_id=%d LIMIT 1", char_id);
                printf("DEBUG: Inventory query: %s\n", query);
                if (mysql_query(conn, query) == 0) {
                    MYSQL_RES *inv_res = mysql_store_result(conn);
                    if (inv_res) {
                        MYSQL_ROW inv_row = mysql_fetch_row(inv_res);
                        if (inv_row) {
                            int inv_num_fields = mysql_num_fields(inv_res);
                            MYSQL_FIELD *inv_fields = mysql_fetch_fields(inv_res);
                            cJSON *bag = cJSON_CreateObject();
                            for (int j = 0; j < inv_num_fields; j++) {
                                if (strncmp(inv_fields[j].name, "bag", 3) == 0) {
                                    int idx = inv_row[j] ? atoi(inv_row[j]) : 0;
                                    printf("DEBUG: %s = %d\n", inv_fields[j].name, idx);
                                    if (idx > 0) {
                                        cJSON_AddNumberToObject(bag, inv_fields[j].name, idx);
                                    } else {
                                        cJSON_AddNullToObject(bag, inv_fields[j].name);
                                    }
                                }
                            }
                            cJSON_AddItemToObject(charobj, "bag", bag);
                        }
                        mysql_free_result(inv_res);
                    }
                }
            }

            // Equipment lookup, numbers only
            if (char_id >= 0) {
                snprintf(query, sizeof(query), "SELECT * FROM character_equipment WHERE char_id=%d LIMIT 1", char_id);
                printf("DEBUG: Equipment query: %s\n", query);
                if (mysql_query(conn, query) == 0) {
                    MYSQL_RES *eq_res = mysql_store_result(conn);
                    if (eq_res) {
                        MYSQL_ROW eq_row = mysql_fetch_row(eq_res);
                        if (eq_row) {
                            int eq_num_fields = mysql_num_fields(eq_res);
                            MYSQL_FIELD *eq_fields = mysql_fetch_fields(eq_res);
                            cJSON *equipment = cJSON_CreateObject();
                            for (int j = 0; j < eq_num_fields; j++) {
                                if (
                                    strcmp(eq_fields[j].name, "id") &&
                                    strcmp(eq_fields[j].name, "char_id")
                                ) {
                                    int eq_idx = eq_row[j] ? atoi(eq_row[j]) : 0;
                                    printf("DEBUG: equipment %s = %d\n", eq_fields[j].name, eq_idx);
                                    if (eq_idx > 0)
                                        cJSON_AddNumberToObject(equipment, eq_fields[j].name, eq_idx);
                                    else
                                        cJSON_AddNullToObject(equipment, eq_fields[j].name);
                                }
                            }
                            cJSON_AddItemToObject(charobj, "equipment", equipment);
                        }
                        mysql_free_result(eq_res);
                    }
                }
            }

            cJSON_AddItemToArray(out, charobj);
            char_found = 1;
        }
        mysql_free_result(res);

        if (!char_found) {
            printf("DEBUG: No characters found for user_id %d\n", user_id);
            send_all(client_fd, "{\"error\":\"No characters found\"}\n", 33);
        } else {
            printf("DEBUG: Characters found and returned.\n");
            char *json = cJSON_PrintUnformatted(out);
            send_all(client_fd, json, strlen(json));
            send_all(client_fd, "\n", 1);
            free(json);
        }
        cJSON_Delete(out);

        close(client_fd);
    }

    mysql_close(conn);
    close(server_fd);
    return 0;
}

