#ifndef _GSC_ENTITY_HPP_
#define _GSC_ENTITY_HPP_

/* gsc functions */
#include "gsc.hpp"

void gsc_entity_setalive(scr_entref_t id);
void gsc_entity_setbounds(scr_entref_t id);
void gsc_entity_spawngrenade(scr_entref_t id);
void gsc_entity_getgrenadefusetime(scr_entref_t id);
void gsc_entity_addgrenadefusetime(scr_entref_t id);
void gsc_entity_isturret(scr_entref_t id);
void gsc_entity_getturretowner(scr_entref_t id);
void gsc_entity_getweaponitemammo(scr_entref_t id);
void gsc_entity_setweaponitemammo(scr_entref_t id);
void gsc_entity_getweaponitemclipammo(scr_entref_t id);
void gsc_entity_setweaponitemclipammo(scr_entref_t id);
void gsc_entity_getitemquantity(scr_entref_t id);
void gsc_entity_setitemquantity(scr_entref_t id);

#endif
