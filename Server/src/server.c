#include "server.h"
#include "actions.h"

/**
 * Initializes the server structure
 */
server_t *server_new(int argc, char *argv[])
{
    server_t *server = calloc(1, sizeof(server_t));
    server->options = options_new(argc, argv);
    server->netctl = netctl_new(server->options->port);
    server->game = init_game(server->options);
    return server;
}

/**
 * Selects the sockets that are ready to be read
 * handle new connection request and read data from clients
 */
void server_select(server_t *server)
{
    size_t elapsed, dt;
    int act;
    fd_set readfds;
    timeval_t *timeout;
    struct timespec start = {0}, end = {0};

    readfds = server->netctl->watched_fd;

    timeout = actions_get_next_timeout(server->actions, server->options->freq);
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    act = select(FD_SETSIZE, &readfds, NULL, NULL, timeout);

    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    free(timeout);

    dt = (end.tv_nsec - start.tv_nsec) + ((end.tv_sec - start.tv_sec) * 1E9);
    elapsed = dt * server->options->freq * 1E-9;
    actions_apply_elapsed_time(server->actions, elapsed);
    if (act < 0)
    {
        printf("Timeout\n");
        return;
    }
    if (FD_ISSET(server->netctl->entrypoint.fd, &readfds))
    {
        printf("New connection\n");
        int fd = netctl_accept(server->netctl);
        server_handshake(server, fd);
    }
    for (list_t head = server->netctl->clients; head != NULL;)
    {
        if (FD_ISSET(((socket_t *)head->value)->fd, &readfds))
        {
            char buffer[1024] = {0};
            ssize_t rbytes = recv(((socket_t *)head->value)->fd, buffer, 1024, 0);
            if (rbytes == 0)
            {
                printf("Client disconnected\n");
                netctl_disconnect(server->netctl, ((socket_t *)head->value)->fd);
                head = server->netctl->clients;
                continue;
            }
            else
            {
                printf("Received %ld bytes\n", rbytes);
                printf("Received: %s\n", buffer);
                actions_accept(&server->actions, action_new(((socket_t *)head->value)->fd, buffer));
            }
        }
        head = head->next;
    }
}

/**
 * Sends the welcome message to the client and waits for the team name
 */
void server_handshake(server_t *server, int fd)
{
    send(fd, "WELCOME\n", 8, 0);
    char buffer[1024] = {0};
    recv(fd, buffer, 1024, 0);
    printf("Handshake done\n");
    printf("Client is on team %s\n", buffer);
    for (list_t head = server->game->teams; head != NULL; head = head->next)
    {
        team_t *team = (team_t *)head->value;
        if (strncmp(team->name, buffer, strlen(team->name)) == 0 && team->nb_players < team->max_players)
        {
            dprintf(fd, "%d\n"
                        "%d %d\n",
                    team->max_players - team->nb_players,
                    server->options->width,
                    server->options->height);
            add_player(server->game, team->name, fd);
            return;
        }
    }
    printf("Team not found\n");
    dprintf(fd, "0\n"
                "0 0\n");
    netctl_disconnect(server->netctl, fd);
}

/**
 * Server main loop
 */
int server_run(server_t *server)
{
    while (1)
    {
        server_select(server);
        for (list_t *head = &server->actions; *head != NULL;)
        {
            action_t *action = (action_t *)(*head)->value;
            printf("Action: %d\n", action->cooldown);
            if (action->cooldown <= 0)
            {
                // action->callback(server->game, action);
                free(action);
                printf("Action done\n");
                list_del_elem_at_front(head);
                continue;
            }
            head = &(*head)->next;
        }
    }

    return 0;
}

/**
 * Frees the server structure
 */
void server_destroy(server_t *server)
{
    options_destroy(server->options);
    netctl_destroy(server->netctl);
    free(server);
}
