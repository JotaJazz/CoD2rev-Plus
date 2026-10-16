#ifndef _GSC_PLAYER_HPP_
#define _GSC_PLAYER_HPP_

/* gsc functions */
#include "gsc.hpp"

void gsc_player_velocity_set(scr_entref_t id);
void gsc_player_velocity_add(scr_entref_t id);
void gsc_player_velocity_get(scr_entref_t id);
void gsc_player_button_ads(scr_entref_t id);
void gsc_player_button_left(scr_entref_t id);
void gsc_player_button_right(scr_entref_t id);
void gsc_player_button_forward(scr_entref_t id);
void gsc_player_button_back(scr_entref_t id);
void gsc_player_button_leanleft(scr_entref_t id);
void gsc_player_button_leanright(scr_entref_t id);
void gsc_player_button_jump(scr_entref_t id);
void gsc_player_button_reload(scr_entref_t id);
void gsc_player_button_frag(scr_entref_t id);
void gsc_player_button_smoke(scr_entref_t id);
void gsc_player_stance_get(scr_entref_t id);
void gsc_player_stance_set(scr_entref_t id);
void gsc_player_spectatorclient_get(scr_entref_t id);
void gsc_player_get_userinfo(scr_entref_t id);
void gsc_player_set_userinfo(scr_entref_t id);
void gsc_player_getip(scr_entref_t id);
void gsc_player_getping(scr_entref_t id);
void gsc_player_clientuserinfochanged(scr_entref_t id);
void gsc_player_clientcommand(scr_entref_t id);
void gsc_player_getlastconnecttime(scr_entref_t id);
void gsc_player_getlastmsg(scr_entref_t id);
void gsc_player_getclientstate(scr_entref_t id);
void gsc_player_addresstype(scr_entref_t id);
void gsc_player_renameclient(scr_entref_t id);
void gsc_player_outofbandprint(scr_entref_t id);
void gsc_player_connectionlesspacket(scr_entref_t id);
void gsc_player_resetnextreliabletime(scr_entref_t id);
void gsc_player_ismantling(scr_entref_t id);
void gsc_player_isonladder(scr_entref_t id);
void gsc_player_isusingturret(scr_entref_t id);
void gsc_player_getjumpslowdowntimer(scr_entref_t id);
void gsc_player_clearjumpstate(scr_entref_t id);
void gsc_player_setg_speed(scr_entref_t id);
void gsc_player_setg_gravity(scr_entref_t id);
void gsc_player_setweaponfiremeleedelay(scr_entref_t id);
void gsc_player_set_anim(scr_entref_t id);
void gsc_player_getcooktime(scr_entref_t id);
void gsc_player_setguid(scr_entref_t id);
void gsc_player_clienthasclientmuted(scr_entref_t id);
void gsc_player_getlastgamestatesize(scr_entref_t id);
void gsc_player_getfps(scr_entref_t id);
void gsc_player_isbot(scr_entref_t id);
void gsc_player_disableitempickup(scr_entref_t id);
void gsc_player_enableitempickup(scr_entref_t id);
void gsc_player_getcurrentoffhandslotammo(scr_entref_t id);
void gsc_player_getprotocol(scr_entref_t id);
void gsc_player_islocalclient(scr_entref_t id);
void gsc_player_isfiring(scr_entref_t id);
void gsc_player_ismeleeing(scr_entref_t id);
void gsc_player_isreloading(scr_entref_t id);
void gsc_player_isthrowinggrenade(scr_entref_t id);
void gsc_player_isusingbinoculars(scr_entref_t id);
void gsc_player_isrechambering(scr_entref_t id);
void gsc_player_isshellshocked(scr_entref_t id);
void gsc_player_ischatting(scr_entref_t id);
void gsc_player_button_holdbreath(scr_entref_t id);
void gsc_player_getcurrentweaponammo(scr_entref_t id);
void gsc_player_getcurrentweaponclipammo(scr_entref_t id);
void gsc_player_setcurrentweaponammo(scr_entref_t id);
void gsc_player_setcurrentweaponclipammo(scr_entref_t id);
void gsc_player_getcurrentweaponslot(scr_entref_t id);
void gsc_player_getspeed(scr_entref_t id);
void gsc_player_getgravity(scr_entref_t id);
void gsc_player_getgroundentity(scr_entref_t id);
void gsc_player_getvieworigin(scr_entref_t id);
void gsc_player_getplayerstateflags(scr_entref_t id);
void gsc_player_getinactivitytime(scr_entref_t id);
void gsc_player_getentertime(scr_entref_t id);
void gsc_player_processsuicide(scr_entref_t id);

// player functions without entity
void gsc_kick_slot();

#endif
