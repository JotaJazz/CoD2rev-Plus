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
void gsc_entity_gettagorigin(scr_entref_t id);
void gsc_entity_gettagangles(scr_entref_t id);
void gsc_entity_hastag(scr_entref_t id);
void gsc_entity_islinkedto(scr_entref_t id);
void gsc_entity_getvmin(scr_entref_t id);
void gsc_entity_getvmax(scr_entref_t id);
void gsc_entity_getclipmask(scr_entref_t id);
void gsc_entity_setclipmask(scr_entref_t id);
void gsc_entity_getcontents(scr_entref_t id);
void gsc_entity_setlight(scr_entref_t id);

#endif
