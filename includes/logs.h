/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logs.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 00:00:00 by kevin             #+#    #+#             */
/*   Updated: 2025/12/17 03:41:17 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOG_H
# define LOG_H

# include <unistd.h>

# define LOG_GREEN  "\033[1;32m"
# define LOG_RED    "\033[1;31m"
# define LOG_YELLOW "\033[1;33m"
# define LOG_BLUE   "\033[1;34m"
# define LOG_GRAY   "\033[0;90m"
# define LOG_CYAN   "\033[1;36m"
# define LOG_MAGENTA "\033[1;35m"
# define LOG_ORANGE "\033[0;93m"
# define LOG_RESET  "\033[0m"

# ifndef LOG_ASCII
#  define LOG_HDR_PARSE   "📜 PARSING 📜"
#  define LOG_HDR_ENGINE  "🔧 ENGINE 🔧"
#  define LOG_HDR_DISPLAY "🖥️ DISPLAY 🖥️"
#  define LOG_TOK_OK      "✓"
#  define LOG_TOK_KO      "✗"
#  define LOG_TOK_WARN    "!"
#  define LOG_TOK_INIT    "…"
#  define LOG_TOK_SKIP    "-"
# else
#  define LOG_HDR_PARSE   "[PARSE] PARSING"
#  define LOG_HDR_ENGINE  "[ENG] ENGINE"
#  define LOG_HDR_DISPLAY "[DISP] DISPLAY"
#  define LOG_TOK_OK      "OK"
#  define LOG_TOK_KO      "KO"
#  define LOG_TOK_WARN    "!"
#  define LOG_TOK_INIT    "..."
#  define LOG_TOK_SKIP    "-"
# endif

# define LOG_PARSE_TOTAL   4
# define LOG_ENGINE_TOTAL  3
# define LOG_DISPLAY_TOTAL 6

typedef enum e_logsec
{
	LOGSEC_BOOT = 0,
	LOGSEC_PARSE = 1,
	LOGSEC_ENGINE = 2,
	LOGSEC_DISPLAY = 3
}	t_logsec;

typedef enum e_logstep
{
	LOGSTEP_BOOT=0,
	LOGSTEP_CHECK_ARGS,
	LOGSTEP_PARSE_INIT_SCENE,
	LOGSTEP_PARSE_READ_FILE,
	LOGSTEP_PARSE_BUILD_CONVERT,
	LOGSTEP_PARSE_FREE_PARSED,
	LOGSTEP_ENGINE_INIT_CAM_VIEWS,
	LOGSTEP_ENGINE_INIT_BVH,
	LOGSTEP_ENGINE_RENDER_INIT,
	LOGSTEP_DISPLAY_INIT_CORE,
	LOGSTEP_DISPLAY_INIT_FRAMES,
	LOGSTEP_DISPLAY_INIT_LOOP,
	LOGSTEP_DISPLAY_LOAD_TEXTURES,
	LOGSTEP_DISPLAY_INIT_CAM_CTRL,
	LOGSTEP_DISPLAY_INIT_SKY
}	t_logstep;

void	log_step(t_logstep step, int status);

typedef struct s_scene t_scene;

void	log_info_parsed(const t_scene *scene, const char *path);

#endif
