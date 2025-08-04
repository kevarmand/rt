#include "rt.h"

int	add_button(t_ui *ui, t_button btn)
{
	if (ui->button_count >= MAX_UI_BUTTONS)
		return (ERROR);

	ui->buttons[ui->button_count] = btn;

	// (Optionnel) Sécurise contre des pointeurs NULL
	if (!btn.txt)
		ui->buttons[ui->button_count].txt = NULL;
	ui->button_count++;
	return (SUCCESS);
}