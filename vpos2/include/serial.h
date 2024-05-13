
#define SERIAL_BUFF_SIZE	100

void putc(char c);
char getc(void);

void vh_serial_init(void);
void vh_serial_irq_enable(void);

extern char serial_buff[SERIAL_BUFF_SIZE];
extern unsigned int push_idx, pop_idx;
