#include <nterm.h>
#include <stdarg.h>
#include <unistd.h>
#include <SDL.h>

char handle_key(SDL_Event *ev);

static void sh_printf(const char *format, ...) {
  static char buf[256] = {};
  va_list ap;
  va_start(ap, format);
  int len = vsnprintf(buf, 256, format, ap);
  va_end(ap);
  term->write(buf, len);
}

static void sh_banner() {
  sh_printf("Built-in Shell in NTerm (NJU Terminal)\n\n");
}

static void sh_prompt() {
  sh_printf("sh> ");
}

static int cmd_echo(char* str) {
	sh_printf("%s", str);
	return 0;
}

static int cmd_run(char* path, char* args) {
	char* pt = path + strlen(path);
	while (*pt != '\n') pt--;
	*pt = '\0';
	char** argv = (char**) malloc(16 * sizeof(char*));
	argv[0] = path;
	for (int i = 0; i < 16; i++) {
		argv[i + 1] = strtok(NULL, " ");
		if (argv[i + 1] == NULL) {
			break;
		}
	}
	printf("0x%x", argv[0]);
	int i = 0;
	execvp(path, argv);
	return 0;
}

static struct {
	const char* name;
	const char* description;
	int (*handler) (char*);
} cmd_table[] = {
	"echo", "display a line of text", cmd_echo
};
static const int NR_CMD = sizeof(cmd_table) / sizeof(cmd_table[0]);
static void sh_handle_cmd(char *_cmd) {
	char* str_end = _cmd + strlen(_cmd);
	char *cmd = strtok(_cmd, " ");
	if (cmd != NULL) {
		char *args = cmd + strlen(_cmd) + 1;
		if (args >= str_end) {
			args = NULL;
		}
		int i;
		for (i = 0; i < NR_CMD; i++) {
			if (strcmp(cmd, cmd_table[i].name) == 0) {
					cmd_table[i].handler(args);
				break;
			}
		}
		if (i == NR_CMD) {
			cmd_run(cmd, args);
			}
		}
	}

void builtin_sh_run() {
	setenv("PATH", "/bin", 0);
  sh_banner();
  sh_prompt();

  while (1) {
    SDL_Event ev;
    if (SDL_PollEvent(&ev)) {
      if (ev.type == SDL_KEYUP || ev.type == SDL_KEYDOWN) {
        const char *res = term->keypress(handle_key(&ev));
        if (res) {
					assert(strlen(res) < 128);
					char buf[128];
					strcpy(buf, res);
          sh_handle_cmd(buf);
          sh_prompt();
        }
      }
    }
    refresh_terminal();
  }
}
