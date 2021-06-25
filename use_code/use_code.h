#ifndef USE_CODE_H /* Only include this file once */
#define USE_CODE_H

#include "../data_both.h"

/* prototypes for all the use functions */

/* 
 * each function is of the same form, that is currently:
 * void use_XXXX(player * , obj *)
 * but might change in the future, so we use a macro to declare them
 */

#define U6O_USE_FUNCTION(x) void use_##x(player* myplr , object* myobj)

U6O_USE_FUNCTION(craft);
U6O_USE_FUNCTION(basic_toggle);
U6O_USE_FUNCTION(toggle);


#endif /*USE_CODE_H*/
