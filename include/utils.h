#ifndef	_UTILS_H
#define	_UTILS_H

extern void delay(unsigned long);
extern void put32(unsigned long, unsigned int);
extern unsigned int get32(unsigned long);
extern int get_el(void);
extern void gen_timer_init(void);
extern void gen_timer_reset(void);


#endif  /*_UTILS_H */