#ifndef SPELL_CODE_H /* Only include this file once */
#define SPELL_CODE_H

#include "../data_both.h"

/* prototypes for all the use functions */

/* 
 * each function is of the same form, that is currently:
 * void spell_XXXX(player * , object *)
 * but might change in the future, so we use a macro to declare them
 */

#define U6O_SPELL_FUNCTION(spell_name) int spell_##spell_name(player* myplr , npc * caster, object* myobj, unsigned short x, unsigned short y)


/* list generated using ../tools/function_list.sh
 */
U6O_SPELL_FUNCTION(create_food);
U6O_SPELL_FUNCTION(detect_magic);
U6O_SPELL_FUNCTION(detect_trap);
U6O_SPELL_FUNCTION(dispel_magic);
U6O_SPELL_FUNCTION(douse);
U6O_SPELL_FUNCTION(explosion);
U6O_SPELL_FUNCTION(harm);
U6O_SPELL_FUNCTION(heal);
U6O_SPELL_FUNCTION(help);
U6O_SPELL_FUNCTION(ignite);
U6O_SPELL_FUNCTION(light);
U6O_SPELL_FUNCTION(infravision);
U6O_SPELL_FUNCTION(magic_arrow);
U6O_SPELL_FUNCTION(poison);
U6O_SPELL_FUNCTION(reappear);
U6O_SPELL_FUNCTION(sleep);
U6O_SPELL_FUNCTION(telekinesis);
U6O_SPELL_FUNCTION(trap);
U6O_SPELL_FUNCTION(unlock_magic);
U6O_SPELL_FUNCTION(untrap);
U6O_SPELL_FUNCTION(vanish);
U6O_SPELL_FUNCTION(armageddon);


#endif /*SPELL_CODE_H*/
