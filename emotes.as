void print(string text) { g_Game.AlertMessage( at_console, text); }
void println(string text) { print(text + "\n"); }

void PluginInit() {
    g_Module.ScriptInfo.SetAuthor( "w00tguy" );
    g_Module.ScriptInfo.SetContactInfo( "https://github.com/wootguy/emotes" );

    g_Hooks.RegisterHook( Hooks::Player::ClientSay, @ClientSay );
	
	loadEmotes();
	setSequencePriorities();
}

enum ANIM_MODES {
	MODE_ONCE, // play once
	MODE_FREEZE, // freeze on the last frame
	MODE_LOOP, // loop betwen start and end frames
	MODE_ILOOP // invert framerate when reaching the start/end frame
}

class Emote {
	int seq;
	int mode;
	float framerate;
	float startFrame;
	float endFrame;
	
	Emote() {}
	
	Emote(int seq, int mode, float framerate, float startFrame, float endFrame) {
		this.seq = seq;
		this.mode = mode;
		this.framerate = framerate;
		this.startFrame = startFrame;
		this.endFrame = endFrame;

		if (framerate == 0) {
			framerate = 0.0000001f;
		}
		if (startFrame <= 0) {
			startFrame = 0.00001f;
		}
		if (endFrame >= 255) {
			endFrame = 254.9999f;
		}
	}
}

array<bool> g_priority_sequences(200); // sequences that have priority over emotes
array<CScheduledFunction@> g_emote_loops(33);

dictionary g_emotes;

void loadEmotes() {
	g_emotes["alpha"] = Emote(187, MODE_FREEZE, 1.45, 180, 236);
	g_emotes["scan"] = Emote(188, MODE_ONCE, 1.0, 0, 255);
	g_emotes["flex"] = Emote(129, MODE_FREEZE, 0.2, 0, 52);
	g_emotes["lewd"] = Emote(88, MODE_ILOOP, 1, 40, 70);
	g_emotes["robot"] = Emote(71, MODE_FREEZE, 1, 0, 100);
	g_emotes["elbow"] = Emote(35, MODE_FREEZE, 1, 135, 135);
	g_emotes["hunch"] = Emote(16, MODE_FREEZE, 1, 40, 98);
	g_emotes["anal"] = Emote(14, MODE_FREEZE, 1, 0, 120);
	g_emotes["joy"] = Emote(9, MODE_FREEZE, 1, 90, 90);
	g_emotes["wave"] = Emote(190, MODE_ONCE, 1.0, 0, 255);
	g_emotes["type"] = Emote(186, MODE_LOOP, 1, 0, 255);
	g_emotes["type2"] = Emote(187, MODE_LOOP, 1.2, 0, 255);
	g_emotes["study"] = Emote(189, MODE_ONCE, 1, 0, 255);
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
	switch(mode) {
		case MODE_ONCE: return "ONCE";
		case MODE_FREEZE: return "FREEZE";
		case MODE_LOOP: return "LOOP";
		case MODE_ILOOP: return "ILOOP";
	}
	return "???";
}

// force animation even when doing other things
void emoteLoop(EHandle h_plr, Emote@ emote, float lastFrame) {
	if (!h_plr.IsValid()) {
		return;
	}
	
	CBasePlayer@ plr = cast<CBasePlayer@>(h_plr.GetEntity());
	if (plr is null or !plr.IsAlive() or !plr.IsConnected()) {
		return;
	}
	
	bool emoteIsPlaying = plr.pev.sequence == emote.seq;
	
	if (!emoteIsPlaying) // player shooting or jumping or something
	{
		if (!g_priority_sequences[plr.pev.sequence]) // sequence that's less important than the emote?
		{
			if (emote.mode == MODE_ILOOP) 
			{
				if (lastFrame >= emote.endFrame-0.1f) 
				{
					lastFrame = emote.endFrame;
					emote.framerate = -abs(emote.framerate);
				} 
				else if (lastFrame <= emote.startFrame+0.1f)
				{
					lastFrame = emote.startFrame;
					emote.framerate = abs(emote.framerate);
				}
			}
			else if (emote.mode == MODE_LOOP)
			{
				lastFrame = emote.startFrame;
			}
			else if (emote.mode == MODE_ONCE) 
			{
				if (lastFrame > emote.endFrame - 0.1f) {
					//println("OK GIVE UP " + lastFrame);
					return;
				}
			}
			else if (emote.mode == MODE_FREEZE)
			{
				if (lastFrame >= emote.endFrame - 0.1f) {
					lastFrame = emote.endFrame;
					emote.framerate = plr.pev.framerate = 0.0000001f;
				}
			}
		
			//println("OMG RESET: " + plr.pev.sequence + "[" + plr.pev.frame + "] -> " + seq + "[" + lastFrame + "] " + framerate);
			
			plr.m_Activity = ACT_RELOAD;
			plr.pev.sequence = emote.seq;
			plr.pev.frame = lastFrame;
			plr.ResetSequenceInfo();
			plr.pev.framerate = emote.framerate;
		} else {
			//println("Sequence " + plr.pev.sequence + " has priority over emotes");
		}
	}
	else // emote sequence playing
	{ 
		bool loopFinished = false;			
		if (emote.mode == MODE_ILOOP)
			loopFinished = (plr.pev.frame - emote.endFrame > 0.01f) or (emote.startFrame - plr.pev.frame > 0.01f);
		else
			loopFinished = emote.framerate > 0 ? (plr.pev.frame - emote.endFrame > 0.01f) : (emote.endFrame - plr.pev.frame > 0.01f);
			
		if (loopFinished)
		{
			if (emote.mode == MODE_ONCE) {
				//println("Emote finished");
				return;
			}
			else if (emote.mode == MODE_FREEZE) {
				//println("Emote freezing " + plr.pev.frame);
				plr.pev.frame = emote.endFrame;
				emote.framerate = plr.pev.framerate = 0.0000001f;
			}
			else if (emote.mode == MODE_LOOP) 
			{
				//println("RESTART SEQ " + plr.pev.frame + " " + framerate);
				plr.pev.frame = emote.startFrame;
			}
			else if (emote.mode == MODE_ILOOP)
			{	
				//println("RESTART SEQ " + plr.pev.frame + " " + framerate);
				if (plr.pev.frame >= emote.endFrame)
					plr.pev.frame = emote.endFrame;
				else if (plr.pev.frame <= emote.startFrame)
					plr.pev.frame = emote.startFrame;

				emote.framerate = -emote.framerate;
				plr.pev.framerate = emote.framerate;
			}
		} 
		else 
		{
			lastFrame = plr.pev.frame;
			
			plr.m_flLastEventCheck = g_Engine.time + 1.0f;
			plr.m_flLastGaitEventCheck = g_Engine.time + 1.0f;
			
			// animation stops at the absolute start/end frames
			if (lastFrame <= 0)
				lastFrame = 0.00001f;
			if (lastFrame >= 255)
				lastFrame = 254.9999f;
		}
	}
	
	@g_emote_loops[plr.entindex()] = g_Scheduler.SetTimeout("emoteLoop", 0, h_plr, @emote, lastFrame);
}

void doEmote(CBasePlayer@ plr, Emote emote) {
	if (!plr.IsAlive()) {
		g_PlayerFuncs.SayText(plr, "Can't play emote while dead.\n");
		return;
	}
	
	g_PlayerFuncs.SayText(plr, 'Sequence: ' + emote.seq + " (" + getModeString(emote.mode) + "), Speed " + emote.framerate + 
		", Frames: " + int(emote.startFrame + 0.5f) + "-" + int(emote.endFrame + 0.5f) + "\n");
	
	plr.m_Activity = ACT_RELOAD;
	plr.pev.frame = emote.startFrame;
	plr.pev.sequence = emote.seq;
	plr.ResetSequenceInfo();
	plr.pev.framerate = emote.framerate;
	
	CScheduledFunction@ func = g_emote_loops[plr.entindex()];
	if (func !is null) { // stop previous emote
		g_Scheduler.RemoveTimer(func);
	}
	@g_emote_loops[plr.entindex()] = g_Scheduler.SetTimeout("emoteLoop", 0, EHandle(plr), emote, emote.startFrame);
}

void doEmoteCommand(CBasePlayer@ plr, const CCommand@ args)
{
	if (args.ArgC() >= 2) 
	{
		string emoteName = args[1].ToLowercase();
		
		bool isNumeric = true;
		for (uint i = 0; i < emoteName.Length(); i++) {
			if (!isdigit(emoteName[i])) {
				isNumeric = false;
				break;
			}
		}
	
		if (isNumeric) // custom emote
		{
			int seq = atoi(args[1]);
		
			int mode = MODE_ONCE;
			string smode = args[2];
			if (smode.ToLowercase() == "loop") {
				mode = MODE_LOOP;
			} else if (smode.ToLowercase() == "iloop") {
				mode = MODE_ILOOP;
			} else if (smode.ToLowercase() == "freeze") {
				mode = MODE_FREEZE;
			}
			
			float framerate = args.ArgC() >= 4 ? atof(args[3]) : 1.0f;
			float startFrame = (framerate >= 0 ? 0.0001f : 254.9999f);
			float endFrame = (framerate >= 0 ? 254.9999f : 0.0001f);
			
			startFrame = args.ArgC() >= 5 ? atof(args[4]) : startFrame;
			endFrame = args.ArgC() >= 6 ? atof(args[5]) : endFrame;

			doEmote(plr, Emote(seq, mode, framerate, startFrame, endFrame));
		}
		else if (emoteName == "list")
		{
			array<string>@ emoteNames = g_emotes.getKeys();
			string emoteText;
			for (uint i = 0; i < emoteNames.length(); i++)
			{
				emoteText += " | " + emoteNames[i];
			}
			if (emoteText.Length() > 0) {
				emoteText = emoteText.SubString(3);
			}
			g_PlayerFuncs.SayText(plr, emoteText + "\n");
		}
		else if (emoteName == "off" or emoteName == "stop") 
		{
			CScheduledFunction@ func = g_emote_loops[plr.entindex()];
			if (func !is null and !func.HasBeenRemoved()) {
				g_Scheduler.RemoveTimer(func);
				plr.m_Activity = ACT_IDLE;
				plr.ResetSequenceInfo();
				g_PlayerFuncs.SayText(plr, "Emote stopped\n");
			} else {
				g_PlayerFuncs.SayText(plr, "No emote is playing\n");
			}
		}
		else // named emote
		{
			if (g_emotes.exists(emoteName)) {
				Emote@ emote = cast<Emote@>( g_emotes[emoteName] );
				float framerate = args.ArgC() >= 3 ? atof(args[2]) : emote.framerate;
				doEmote(plr, emote);
			} else {
				g_PlayerFuncs.SayText(plr, "No emote found with name " + emoteName + "\n");
			}
		}
	}
	else
	{
		g_PlayerFuncs.SayText(plr, 'Say ".e list" to list named emotes.\n');
		g_PlayerFuncs.SayText(plr, 'Say ".e off" to stop your emote.\n');
		g_PlayerFuncs.SayText(plr, 'Say ".e <name> [speed]" to play a named emote.\n');
		g_PlayerFuncs.SayText(plr, 'Say ".e <sequence> [mode] [speed] [start_frame] [end_frame]" for more control\n');
		g_PlayerFuncs.SayText(plr, '[speed] is uncapped, [frames] are 0-255, [sequences] end at 190 for most models\n');
		g_PlayerFuncs.SayText(plr, '[mode] can be: ONCE, FREEZE, LOOP, ILOOP\n');
	}
}

HookReturnCode ClientSay( SayParameters@ pParams )
{
	CBasePlayer@ plr = pParams.GetPlayer();
	const CCommand@ args = pParams.GetArguments();
	if (args.ArgC() > 0 and (args[0].Find(".emote") == 0 or args[0] == '.e'))
	{
		doEmoteCommand(plr, args);
		pParams.ShouldHide = true;
		return HOOK_HANDLED;
	}
	return HOOK_CONTINUE;
}