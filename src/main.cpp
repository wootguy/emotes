#include "main.h"
#include <algorithm>

// TODO: ghosts use iuser4=1 instead of noise1=emote

using namespace std;

// Description of plugin
plugin_info_t Plugin_info = {
	META_INTERFACE_VERSION,	// ifvers
	"Emotes",	// name
	"1.0",	// version
	__DATE__,	// date
	"w00tguy",	// author
	"https://github.com/wootguy/",	// url
	"EMOTES",	// logtag, all caps please
	PT_ANYTIME,	// (when) loadable
	PT_ANYPAUSE,	// (when) unloadable
};

vector<bool> g_priority_sequences(200); // sequences that have priority over emotes
vector<ScheduledFunction> g_emote_loops(33);

map<string, Emote> g_emotes;

void loadEmotes() {
	g_emotes["alpha"] = Emote({
		EmotePart(187, MODE_FREEZE, 1.45, 180, 236)
		}, false);
	g_emotes["scan"] = Emote({
		EmotePart(188, MODE_ONCE, 1.0, 0, 255)
		}, false);
	g_emotes["flex"] = Emote({
		EmotePart(129, MODE_FREEZE, 0.2, 0, 52)
		}, false);
	g_emotes["lewd"] = Emote({
		EmotePart(88, MODE_ILOOP, 1, 40, 70)
		}, false);
	g_emotes["robot"] = Emote({
		EmotePart(71, MODE_FREEZE, 1, 0, 100)
		}, false);
	g_emotes["elbow"] = Emote({
		EmotePart(35, MODE_FREEZE, 1, 135, 135)
		}, false);
	g_emotes["hunch"] = Emote({
		EmotePart(16, MODE_FREEZE, 1, 40, 98)
		}, false);
	g_emotes["anal"] = Emote({
		EmotePart(14, MODE_FREEZE, 1, 0, 120)
		}, false);
	g_emotes["joy"] = Emote({
		EmotePart(9, MODE_FREEZE, 1, 90, 90)
		}, false);
	g_emotes["wave"] = Emote({
		EmotePart(190, MODE_ONCE, 1.0, 0, 255)
		}, false);
	g_emotes["type"] = Emote({
		EmotePart(186, MODE_LOOP, 1, 0, 255)
		}, false);
	g_emotes["type2"] = Emote({
		EmotePart(187, MODE_LOOP, 1.2, 0, 255)
		}, false);
	g_emotes["study"] = Emote({
		EmotePart(189, MODE_ONCE, 1, 0, 255)
		}, false);
	g_emotes["oof"] = Emote({
		EmotePart(13, MODE_ONCE, 1, 0, 255),
		EmotePart(14, MODE_ONCE, -1, 255, 0)
		}, false);
	g_emotes["dance"] = Emote({
		EmotePart(31, MODE_ILOOP, 1, 35, 255)
		}, false);
	g_emotes["dance2"] = Emote({
		EmotePart(71, MODE_ILOOP, 1, 0, 220)
		}, false);
	g_emotes["shake"] = Emote({
		EmotePart(106, MODE_FREEZE, 1, 0, 0)
		}, false);
	g_emotes["fidget"] = Emote({
		EmotePart(50, MODE_ILOOP, 1, 100, 245)
		}, false);
	g_emotes["barnacle"] = Emote({
		EmotePart(182, MODE_ONCE, 1, 0, 255),
		EmotePart(183, MODE_ONCE, 1, 0, 255),
		EmotePart(184, MODE_ONCE, 1, 0, 255),
		EmotePart(185, MODE_LOOP, 1, 0, 255)
		}, false);
	g_emotes["swim"] = Emote({
		EmotePart(11, MODE_LOOP, 1, 0, 255)
		}, false);
	g_emotes["swim2"] = Emote({
		EmotePart(10, MODE_LOOP, 1, 0, 255)
		}, false);
	g_emotes["run"] = Emote({
		EmotePart(3, MODE_LOOP, 1, 0, 255)
		}, false);
	g_emotes["crazy"] = Emote({
		EmotePart(183, MODE_LOOP, 4, 0, 255)
		}, false);
}

void setSequencePriorities() {
	//g_priority_sequences[8] = true; // jump
	g_priority_sequences[9] = true; // longjump

	g_priority_sequences[12] = true; // death
	g_priority_sequences[13] = true; // death
	g_priority_sequences[14] = true; // death
	g_priority_sequences[15] = true; // death
	g_priority_sequences[16] = true; // death
	g_priority_sequences[17] = true; // death
	g_priority_sequences[18] = true; // death

	g_priority_sequences[19] = true; // draw crowbar
	g_priority_sequences[21] = true; // shoot crowbar
	g_priority_sequences[22] = true; // draw crowbar (crouched)
	g_priority_sequences[24] = true; // shoot crowbar (crouched)

	g_priority_sequences[25] = true; // cock wrench
	g_priority_sequences[26] = true; // hold wrench
	g_priority_sequences[27] = true; // shoot wrench
	g_priority_sequences[28] = true; // cock wrench (crouched)
	g_priority_sequences[29] = true; // hold wrench (crouched)
	g_priority_sequences[30] = true; // shoot wrench (crouched)

	g_priority_sequences[31] = true; // draw grenade
	g_priority_sequences[33] = true; // cock grenade
	g_priority_sequences[34] = true; // hold grenade
	g_priority_sequences[35] = true; // throw grenade
	g_priority_sequences[36] = true; // draw grenade (crouched)
	g_priority_sequences[38] = true; // cock grenade (crouched)
	g_priority_sequences[39] = true; // hold grenade (crouched)
	g_priority_sequences[40] = true; // throw grenade (crouched)

	g_priority_sequences[41] = true; // draw tripmine
	g_priority_sequences[43] = true; // shoot tripmine
	g_priority_sequences[44] = true; // draw tripmine (crouched)
	g_priority_sequences[46] = true; // draw tripmine (crouched)

	g_priority_sequences[47] = true; // draw onehanded
	g_priority_sequences[49] = true; // shoot onehanded
	g_priority_sequences[50] = true; // reload onehanded
	g_priority_sequences[51] = true; // draw onehanded (crouched)
	g_priority_sequences[53] = true; // shoot onehanded (crouched)
	g_priority_sequences[54] = true; // reload onehanded (crouched)

	g_priority_sequences[55] = true; // draw python
	g_priority_sequences[57] = true; // shoot python
	g_priority_sequences[58] = true; // reload python
	g_priority_sequences[59] = true; // draw python (crouched)
	g_priority_sequences[61] = true; // shoot python (crouched)
	g_priority_sequences[62] = true; // reload python (crouched)

	g_priority_sequences[63] = true; // draw shotgun
	g_priority_sequences[65] = true; // shoot shotgun
	g_priority_sequences[66] = true; // reload shotgun
	g_priority_sequences[67] = true; // draw shotgun (crouched)
	g_priority_sequences[69] = true; // shoot shotgun (crouched)
	g_priority_sequences[70] = true; // reload shotgun (crouched)

	g_priority_sequences[71] = true; // draw gauss
	g_priority_sequences[73] = true; // shoot gauss
	g_priority_sequences[74] = true; // draw gauss (crouched)
	g_priority_sequences[76] = true; // shoot gauss (crouched)

	g_priority_sequences[77] = true; // draw mp5
	g_priority_sequences[79] = true; // shoot mp5
	g_priority_sequences[80] = true; // reload mp5
	g_priority_sequences[81] = true; // draw mp5 (crouched)
	g_priority_sequences[83] = true; // shoot mp5 (crouched)
	g_priority_sequences[84] = true; // reload mp5 (crouched)

	g_priority_sequences[85] = true; // draw mp5
	g_priority_sequences[87] = true; // shoot mp5
	g_priority_sequences[88] = true; // reload mp5
	g_priority_sequences[89] = true; // draw mp5 (crouched)
	g_priority_sequences[91] = true; // shoot mp5 (crouched)
	g_priority_sequences[92] = true; // reload mp5 (crouched)

	g_priority_sequences[93] = true; // draw egon
	g_priority_sequences[95] = true; // shoot egon
	g_priority_sequences[96] = true; // draw egon (crouched)
	g_priority_sequences[98] = true; // shoot egon (crouched)

	g_priority_sequences[99] = true; // draw squeak/snark
	g_priority_sequences[101] = true; // shoot squeak/snark
	g_priority_sequences[102] = true; // draw squeak/snark (crouched)
	g_priority_sequences[104] = true; // shoot squeak/snark (crouched)

	g_priority_sequences[105] = true; // draw hive/hornet
	g_priority_sequences[107] = true; // shoot hive/hornet
	g_priority_sequences[108] = true; // draw hive/hornet (crouched)
	g_priority_sequences[110] = true; // shoot hive/hornet (crouched)

	g_priority_sequences[111] = true; // draw bow
	g_priority_sequences[113] = true; // shoot bow
	g_priority_sequences[115] = true; // shoot bow (scoped)
	g_priority_sequences[116] = true; // reload
	g_priority_sequences[117] = true; // draw bow (crouched)
	g_priority_sequences[119] = true; // shoot bow (crouched)
	g_priority_sequences[121] = true; // shoot bow (crouched+scoped)
	g_priority_sequences[122] = true; // reload (crouched)

	g_priority_sequences[123] = true; // draw minigun
	g_priority_sequences[125] = true; // shoot minigun
	g_priority_sequences[126] = true; // draw minigun (crouched)
	g_priority_sequences[128] = true; // shoot minigun (crouched)

	g_priority_sequences[129] = true; // draw uzis
	g_priority_sequences[130] = true; // draw left uzi
	g_priority_sequences[132] = true; // shoot uzis
	g_priority_sequences[133] = true; // shoot right uzi
	g_priority_sequences[134] = true; // shoot left uzi
	g_priority_sequences[135] = true; // reload right uzi
	g_priority_sequences[136] = true; // reload left uzi
	g_priority_sequences[137] = true; // draw uzis
	g_priority_sequences[138] = true; // draw left uzi
	g_priority_sequences[140] = true; // shoot uzis
	g_priority_sequences[141] = true; // shoot right uzi
	g_priority_sequences[142] = true; // shoot left uzi
	g_priority_sequences[143] = true; // reload right uzi
	g_priority_sequences[144] = true; // reload left uzi

	g_priority_sequences[145] = true; // draw m16
	g_priority_sequences[147] = true; // shoot m16
	g_priority_sequences[148] = true; // shoot m203
	g_priority_sequences[149] = true; // reload m16
	g_priority_sequences[150] = true; // reload m203
	g_priority_sequences[151] = true; // draw m16
	g_priority_sequences[153] = true; // shoot m16
	g_priority_sequences[154] = true; // shoot m203
	g_priority_sequences[155] = true; // reload m16
	g_priority_sequences[156] = true; // reload m203

	g_priority_sequences[157] = true; // draw sniper
	g_priority_sequences[159] = true; // shoot sniper
	g_priority_sequences[161] = true; // shoot sniper (scoped)
	g_priority_sequences[162] = true; // reload sniper
	g_priority_sequences[163] = true; // draw sniper
	g_priority_sequences[165] = true; // shoot sniper
	g_priority_sequences[167] = true; // shoot sniper (scoped)
	g_priority_sequences[168] = true; // reload sniper

	g_priority_sequences[169] = true; // draw saw
	g_priority_sequences[171] = true; // shoot saw
	g_priority_sequences[172] = true; // reload saw
	g_priority_sequences[173] = true; // draw saw
	g_priority_sequences[175] = true; // shoot saw
	g_priority_sequences[176] = true; // reload saw

	g_priority_sequences[182] = true; // barnacle hit
	g_priority_sequences[183] = true; // barnacle pull
	g_priority_sequences[184] = true; // barnacle crunch
	g_priority_sequences[185] = true; // barnacle chew
}

string getModeString(int mode) {
	switch (mode) {
	case MODE_ONCE: return "ONCE";
	case MODE_FREEZE: return "FREEZE";
	case MODE_LOOP: return "LOOP";
	case MODE_ILOOP: return "ILOOP";
	}
	return "???";
}

// force animation even when doing other things
void emoteLoop(EHandle h_plr, EHandle h_target, Emote emote, int partIdx, float lastFrame) {
	if (!h_plr.IsValid()) {
		return;
	}

	CBasePlayer* plr = (CBasePlayer*)(h_plr.GetEntity());
	if (!plr || !plr->IsConnected()) {
		return;
	}

	CBaseMonster* target =(CBaseMonster*)(h_target.GetEntity());
	if (!target) {
		return;
	}

	bool targetIsGhost = target->entindex() != plr->entindex();
	if (!plr->IsAlive() && !targetIsGhost) { // stop if player was killed
		return;
	}

	EmotePart e = emote.parts[partIdx];

	bool emoteIsPlaying = target->pev->sequence == e.seq;

	if (!emoteIsPlaying) // player shooting || jumping || something
	{
		if (!g_priority_sequences[target->pev->sequence]) // sequence that's less important than the emote?
		{
			if (e.mode == MODE_ILOOP)
			{
				if (lastFrame >= e.endFrame - 0.1f)
				{
					lastFrame = e.endFrame;
					e.framerate = -abs(e.framerate);
				}
				else if (lastFrame <= e.startFrame + 0.1f)
				{
					lastFrame = e.startFrame;
					e.framerate = abs(e.framerate);
				}
			}
			else if (e.mode == MODE_LOOP)
			{
				lastFrame = e.startFrame;
			}
			else if (e.mode == MODE_ONCE)
			{
				if ((e.framerate >= 0 && lastFrame > e.endFrame - 0.1f) ||
					(e.framerate < 0 && lastFrame < e.endFrame + 0.1f) ||
					(e.framerate >= 0 && target->pev->frame < lastFrame) ||
					(e.framerate < 0 && target->pev->frame > lastFrame)) {
					//println("OK GIVE UP " + lastFrame);
					doEmote(plr, emote, partIdx + 1);
					return;
				}
			}
			else if (e.mode == MODE_FREEZE)
			{
				if ((e.framerate >= 0 && lastFrame >= e.endFrame - 0.1f) ||
					(e.framerate < 0 && lastFrame <= e.endFrame + 0.1f)) {
					lastFrame = e.endFrame;
					e.framerate = target->pev->framerate = 0.0000001f;
				}
			}

			//println("OMG RESET: " + target->pev->sequence + "[" + target->pev->frame + "] -> " + e.seq + "[" + lastFrame + "] " + e.framerate);

			target->m_Activity = ACT_RELOAD;
			target->pev->sequence = e.seq;
			target->pev->frame = lastFrame;
			target->ResetSequenceInfo();
			target->pev->framerate = e.framerate;
		}
		else {
			//println("Sequence " + plr->pev->sequence + " has priority over emotes");
		}
	}
	else // emote sequence playing
	{
		bool loopFinished = false;
		if (e.mode == MODE_ILOOP)
			loopFinished = (target->pev->frame - e.endFrame > 0.01f) || (e.startFrame - target->pev->frame > 0.01f);
		else
			loopFinished = e.framerate > 0 ? (target->pev->frame - e.endFrame > 0.01f) : (e.endFrame - target->pev->frame > 0.01f);

		if (loopFinished)
		{
			if (e.mode == MODE_ONCE) {
				//println("Emote finished");
				doEmote(plr, emote, partIdx + 1);
				return;
			}
			else if (e.mode == MODE_FREEZE) {
				//println("Emote freezing " + plr->pev->frame);
				target->pev->frame = e.endFrame;
				e.framerate = target->pev->framerate = 0.0000001f;
			}
			else if (e.mode == MODE_LOOP)
			{
				//println("RESTART SEQ " + plr->pev->frame + " " + framerate);
				target->pev->frame = e.startFrame;
			}
			else if (e.mode == MODE_ILOOP)
			{
				//println("RESTART SEQ " + plr->pev->frame + " " + e.framerate);
				lastFrame = target->pev->frame;
				if (lastFrame >= e.endFrame - 0.1f)
				{
					lastFrame = e.endFrame;
					e.framerate = -abs(e.framerate);
				}
				else if (lastFrame <= e.startFrame + 0.1f)
				{
					lastFrame = e.startFrame;
					e.framerate = abs(e.framerate);
				}

				target->pev->framerate = e.framerate;
			}
		}
		else
		{
			lastFrame = target->pev->frame;

			target->m_flLastEventCheck = gpGlobals->time + 1.0f;
			target->m_flLastGaitEventCheck = gpGlobals->time + 1.0f;

			// animation stops at the absolute start/end frames
			if (lastFrame <= 0)
				lastFrame = 0.00001f;
			if (lastFrame >= 255)
				lastFrame = 254.9999f;
		}
	}

	g_emote_loops[plr->entindex()] = g_Scheduler.SetTimeout(emoteLoop, 0, h_plr, h_target, emote, partIdx, lastFrame);
}

// compatibililty with the ghosts plugin
CBaseMonster* getGhostEnt(CBasePlayer* plr) {
	string id = getPlayerUniqueId(plr->edict());

	edict_t* ent = NULL;
	do {
		ent = g_engfuncs.pfnFindEntityByString(ent, "classname", "monster_ghost");
		if (isValidFindEnt(ent))
		{
			if (string(STRING(ent->v.noise)) == id && ent->v.flags & FL_MONSTER) {
				return (CBaseMonster*)GET_PRIVATE(ent);
			}
		}
	} while (isValidFindEnt(ent));

	return NULL;
}

void doEmote(CBasePlayer* plr, Emote emote, int partIdx) {
	CBaseMonster* emoteEnt = (CBaseMonster*)(plr);

	if (!plr->IsAlive()) {
		emoteEnt = getGhostEnt(plr);
		if (!emoteEnt) {
			ClientPrint(plr->edict(), HUD_PRINTTALK, "Can't play emote while dead.\n");
			return;
		}
		else {
			emoteEnt->pev->iuser4 = 1; // tell the ghosts plugin that an emote is playing
		}
	}
	if (partIdx >= int(emote.parts.size())) {
		if (emote.loop) {
			partIdx = 0;
		}
		else {
			return;
		}
	}

	EmotePart e = emote.parts[partIdx];
	ClientPrint(plr->edict(), HUD_PRINTCONSOLE, ("Part: " + to_string(partIdx) + ", Sequence : "
		+ to_string(e.seq) + " (" + getModeString(e.mode) + ")" + ", Speed " + to_string(e.framerate)
		+ ", Frames: " + to_string(int(e.startFrame + 0.5f))
		+ "-" + to_string(int(e.endFrame + 0.5f)) + "\n").c_str());

	emoteEnt->m_Activity = ACT_RELOAD;
	emoteEnt->pev->frame = e.startFrame;
	emoteEnt->pev->sequence = e.seq;
	emoteEnt->ResetSequenceInfo();
	emoteEnt->pev->framerate = e.framerate;

	ScheduledFunction func = g_emote_loops[plr->entindex()];
	if (!func.HasBeenRemoved()) { // stop previous emote
		g_Scheduler.RemoveTimer(func);
	}
	g_emote_loops[plr->entindex()] = g_Scheduler.SetTimeout(emoteLoop, 0, EHandle(plr), EHandle(emoteEnt), emote, partIdx, e.startFrame);
}

bool doCommand(edict_t* eplr) {
	CommandArgs args = CommandArgs();
	args.loadArgs();

	if (args.ArgC() == 0 || args.ArgV(0) != ".e") {
		return false;
	}

	CBasePlayer* plr = (CBasePlayer*)GET_PRIVATE(eplr);

	if (args.ArgC() >= 2)
	{
		string emoteName = toLowerCase(args.ArgV(1));

		bool isNumeric = true;
		for (int i = 0; i < emoteName.size(); i++) {
			if (!isdigit(emoteName[i])) {
				isNumeric = false;
				break;
			}
		}

		if (emoteName == "version") {
			ClientPrint(eplr, HUD_PRINTTALK, "emotes plugin v2\n");
		}
		else if (emoteName == "chain") // super custom emote
		{
			float speedMod = atof(args.ArgV(2).c_str());
			string loopMode = toLowerCase(args.ArgV(3));

			int lastSeqMode = MODE_ONCE;
			if (loopMode == "loopend") {
				lastSeqMode = MODE_LOOP;
			}
			if (loopMode == "iloopend") {
				lastSeqMode = MODE_ILOOP;
			}
			if (loopMode == "freezeend") {
				lastSeqMode = MODE_FREEZE;
			}

			vector<EmotePart> parts;

			for (int i = 4; i < args.ArgC(); i++) {
				vector<string> seqOpts = splitString(args.ArgV(i), "_");

				int seq = atoi(seqOpts[0].c_str());
				float speed = (seqOpts.size() > 1 ? atof(seqOpts[1].c_str()) : 1) * speedMod;

				float startFrame = (speed >= 0 ? 0.0001f : 254.9999f);
				float endFrame = (speed >= 0 ? 254.9999f : 0.0001f);
				startFrame = seqOpts.size() > 2 ? atof(seqOpts[2].c_str()) : startFrame;
				endFrame = seqOpts.size() > 3 ? atof(seqOpts[3].c_str()) : endFrame;

				if (seq > 255) {
					seq = 255;
				}

				bool isLast = i == args.ArgC() - 1;
				int mode = isLast ? lastSeqMode : MODE_ONCE;

				parts.push_back(EmotePart(seq, mode, speed, startFrame, endFrame));
			}

			doEmote(plr, Emote(parts, loopMode == "loop"), 0);
		}
		else if (isNumeric) // custom emote
		{
			int seq = atoi(args.ArgV(1).c_str());

			int mode = MODE_ONCE;
			string smode = args.ArgV(2);
			if (toLowerCase(smode) == "loop") {
				mode = MODE_LOOP;
			}
			else if (toLowerCase(smode) == "iloop") {
				mode = MODE_ILOOP;
			}
			else if (toLowerCase(smode) == "freeze") {
				mode = MODE_FREEZE;
			}

			float framerate = args.ArgC() >= 4 ? atof(args.ArgV(3).c_str()) : 1.0f;
			float startFrame = (framerate >= 0 ? 0.0001f : 254.9999f);
			float endFrame = (framerate >= 0 ? 254.9999f : 0.0001f);
			if (seq > 255) {
				seq = 255;
			}

			startFrame = args.ArgC() >= 5 ? atof(args.ArgV(4).c_str()) : startFrame;
			endFrame = args.ArgC() >= 6 ? atof(args.ArgV(5).c_str()) : endFrame;

			doEmote(plr, Emote({ EmotePart(seq, mode, framerate, startFrame, endFrame) }, false), 0);
		}
		else if (emoteName == "list")
		{
			vector<string> emoteNames;
			for (auto iter : g_emotes) {
				emoteNames.push_back(iter.first);
			}
			sort(emoteNames.begin(), emoteNames.end());

			string emoteText;
			for (int i = 0; i < emoteNames.size(); i++)
			{
				emoteText += " | " + emoteNames[i];
			}
			emoteText = "Emotes: " + emoteText.substr(3);

			if (args.isConsoleCmd)
				ClientPrint(eplr, HUD_PRINTCONSOLE, (emoteText + "\n").c_str());
			else
				ClientPrint(eplr, HUD_PRINTTALK, (emoteText + "\n").c_str());
		}
		else if (emoteName == "off" || emoteName == "stop")
		{
			ScheduledFunction func = g_emote_loops[plr->entindex()];
			if (!func.HasBeenRemoved()) {
				g_Scheduler.RemoveTimer(func);
				plr->m_Activity = ACT_IDLE;
				plr->ResetSequenceInfo();

				CBaseEntity* ghostEnt = getGhostEnt(plr);
				if (ghostEnt) {
					ghostEnt->pev->iuser4 = 0; // tell the ghsots plugin that the emote was stopped
				}

				ClientPrint(eplr, HUD_PRINTTALK, "Emote stopped\n");
			}
			else {
				ClientPrint(eplr, HUD_PRINTTALK, "No emote is playing\n");
			}
		}
		else // named emote
		{
			if (g_emotes.count(emoteName)) {
				Emote& emote = g_emotes[emoteName];

				float speed = args.ArgC() >= 3 ? atof(args.ArgV(2).c_str()) : 1.0f;
				for (int i = 0; i < emote.parts.size(); i++) {
					emote.parts[i].framerate *= speed;
				}

				doEmote(plr, emote, 0);
			}
			else {
				ClientPrint(eplr, HUD_PRINTTALK, ("No emote found with name " + emoteName + "\n").c_str());
			}
		}
	}
	else
	{
		if (!args.isConsoleCmd)
		{
			ClientPrint(eplr, HUD_PRINTTALK, "Say \".e list\" to list named emotes.\n");
			ClientPrint(eplr, HUD_PRINTTALK, "Say \".e off\" to stop your emote.\n");
			ClientPrint(eplr, HUD_PRINTTALK, "Say \".e <name> [speed]\" to play a named emote.\n");
			ClientPrint(eplr, HUD_PRINTTALK, "Say \".e <sequence> [mode] [speed] [start_frame] [end_frame]\" for more control.\n");
			ClientPrint(eplr, HUD_PRINTTALK, "Type \".e\" in console for more info.");
		}
		else
		{
			ClientPrint(eplr, HUD_PRINTCONSOLE, "----------------------------------Emote Commands----------------------------------\n\n");
			ClientPrint(eplr, HUD_PRINTCONSOLE, "Type \".e list\" to list named emotes.\n");
			ClientPrint(eplr, HUD_PRINTCONSOLE, "Type \".e off\" to stop your emote.\n");
			ClientPrint(eplr, HUD_PRINTCONSOLE, "Type \".e <name> [speed]\" to play a named emote.\n");
			ClientPrint(eplr, HUD_PRINTCONSOLE, "Type \".e <sequence> [mode] [speed] [start_frame] [end_frame]\" for more control.\n");
			ClientPrint(eplr, HUD_PRINTCONSOLE, "Type \".e chain <speed> <chain_mode> <seq>_[speed]_[start_frame]_[end_frame] ...\" for advanced combos.\n");

			ClientPrint(eplr, HUD_PRINTCONSOLE, "\n<> = required. [] = optional.\n\n");

			ClientPrint(eplr, HUD_PRINTCONSOLE, "<sequence> = 0-255. Most models have about 190 sequences.\n");
			ClientPrint(eplr, HUD_PRINTCONSOLE, "[mode] = once, freeze, loop, || iloop.\n");
			ClientPrint(eplr, HUD_PRINTCONSOLE, "[chain_mode] = once, loop, freezeend, loopend, || iloopend.\n");
			ClientPrint(eplr, HUD_PRINTCONSOLE, "[speed] = Any number, even negative. The default speed is 1.\n");
			ClientPrint(eplr, HUD_PRINTCONSOLE, "[start_frame/end_frame] = 0-255. This is like a percentage. Frame count in the model doesn\"t matter.\n");

			ClientPrint(eplr, HUD_PRINTCONSOLE, "\nExamples:\n");
			ClientPrint(eplr, HUD_PRINTCONSOLE, ".e oof\n");
			ClientPrint(eplr, HUD_PRINTCONSOLE, ".e oof 2\n");
			ClientPrint(eplr, HUD_PRINTCONSOLE, ".e 15 iloop\n");
			ClientPrint(eplr, HUD_PRINTCONSOLE, ".e 15 iloop 0.5\n");
			ClientPrint(eplr, HUD_PRINTCONSOLE, ".e 15 iloop 0.5 0 50\n");
			ClientPrint(eplr, HUD_PRINTCONSOLE, ".e chain 2 loop 13 14 15\n");
			ClientPrint(eplr, HUD_PRINTCONSOLE, ".e chain 1 once 13 14_-1\n");
			ClientPrint(eplr, HUD_PRINTCONSOLE, ".e chain 1 iloopend 182 183 184 185\n");
			ClientPrint(eplr, HUD_PRINTCONSOLE, ".e chain 1 freezeend 15_0.1_0_50 16_-1_100_10\n");

			ClientPrint(eplr, HUD_PRINTCONSOLE, "\n----------------------------------------------------------------------------------\n");
		}
	}

	return true;
}

// called before angelscript hooks
void ClientCommand(edict_t* pEntity) {
	META_RES ret = doCommand(pEntity) ? MRES_SUPERCEDE : MRES_IGNORED;
	RETURN_META(ret);
}

void StartFrame() {
	g_Scheduler.Think();
	RETURN_META(MRES_IGNORED);
}

void PluginInit() {	
	g_dll_hooks.pfnClientCommand = ClientCommand;
	g_dll_hooks.pfnStartFrame = StartFrame;

	loadEmotes();
	setSequencePriorities();
}

void PluginExit() {}