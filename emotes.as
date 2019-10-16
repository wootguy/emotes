void print(string text) { g_Game.AlertMessage( at_console, text); }
void println(string text) { print(text + "\n"); }

void PluginInit() {
    g_Module.ScriptInfo.SetAuthor( "w00tguy" );
    g_Module.ScriptInfo.SetContactInfo( "https://github.com/wootguy/emotes" );

    g_Hooks.RegisterHook( Hooks::Player::ClientSay, @ClientSay );
	
	loadEmotes();
}

class Emote {
	int seq;
	float frame;
	float framerate;
	float freezeFrame;
	
	Emote() {}
	
	Emote(int seq, float frame, float framerate, float freezeFrame) {
		this.seq = seq;
		this.frame = frame;
		this.framerate = framerate;
		this.freezeFrame = freezeFrame;
		if (this.framerate <= 0) {
			this.framerate = 0.0000001f;
		}
	}
}

dictionary g_emotes;

void loadEmotes() {
	g_emotes["research"] = Emote(189, 170, 0.75, -1);
	g_emotes["alpha"] = Emote(187, 180, 1.45, 236);
	g_emotes["scan"] = Emote(188, 0, 1.0, -1);
	g_emotes["flex"] = Emote(129, 0, 0.2, 52);
	g_emotes["lewd"] = Emote(88, 20, 0.1, 60);
	g_emotes["robot"] = Emote(71, 100, 0, -1);
	g_emotes["elbow"] = Emote(35, 135, 0, -1);
	g_emotes["hunch"] = Emote(16, 98, 0, -1);
	g_emotes["anal"] = Emote(14, 120, 0, -1);
	g_emotes["rejoice"] = Emote(9, 90, 0, -1);
	g_emotes["wave"] = Emote(190, 0, 1.0, -1);
	g_emotes["type"] = Emote(186, 0, 1.0, -1);
}

// stop animation at a specific frame
void player_pose_freeze_frame(EHandle h_plr, int seq, float frame) {
	if (!h_plr.IsValid()) {
		return;
	}
	
	CBaseEntity@ plr = h_plr;
	if (plr.pev.sequence != seq) {
		return;
	}
	
	if (plr.pev.frame >= frame) {
		plr.pev.frame = frame;
		plr.pev.framerate = 0.00001f;
		return;
	}
	
	g_Scheduler.SetTimeout("player_pose_freeze_frame", 0, h_plr, seq, frame);
}

void player_pose(CBasePlayer@ plr, int seq, float frame, float framerate, float freezeFrame=-1) {
	if (framerate == 0) {
		framerate = 0.00001;
	}
	
	g_PlayerFuncs.SayText(plr, 'Sequence: ' + seq + ", Frame " + frame + ", Speed " + framerate + ", Freeze-frame: " + freezeFrame);
	
	plr.m_Activity = ACT_RELOAD;
	plr.pev.frame = frame;
	plr.pev.sequence = seq;
	plr.ResetSequenceInfo();
	plr.pev.framerate = framerate;
	if (freezeFrame != -1) {
		player_pose_freeze_frame(EHandle(plr), seq, freezeFrame);
	}
}

void doEmoteCommand(CBasePlayer@ plr, const CCommand@ args)
{	
	
	if (args.ArgC() == 1 and args[0] == '.emote_list')
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
		g_PlayerFuncs.SayText(plr, emoteText);
	} 
	else if (args.ArgC() == 5) {
		int seq = atoi(args[1]);
		float frame = atof(args[2]);
		float framerate = atof(args[3]);
		float freezeFrame = atof(args[4]);
		player_pose(plr, seq, frame, framerate, freezeFrame);
	}
	else if (args.ArgC() == 2)
	{
		string emoteName = args[1];
		
		bool isNumeric = true;
		for (uint i = 0; i < emoteName.Length(); i++) {
			if (!isdigit(emoteName[i])) {
				isNumeric = false;
				break;
			}
		}
		
		if (isNumeric) {
			player_pose(plr, atoi(emoteName), 0, 1, -1);
		}
		else if (g_emotes.exists(emoteName)) {
			Emote@ emote = cast<Emote@>( g_emotes[emoteName] );
			player_pose(plr, emote.seq, emote.frame, emote.framerate, emote.freezeFrame);
		} else {
			g_PlayerFuncs.SayText(plr, "No emote found with name " + emoteName);
		}
	} 
	else
	{
		g_PlayerFuncs.SayText(plr, 'Say ".emote_list" to list named emotes.\n');
		g_PlayerFuncs.SayText(plr, 'Say ".e [name]" to play a named emote.\n');
		g_PlayerFuncs.SayText(plr, 'Say ".e [number]" to play a specific animation.\n');
		g_PlayerFuncs.SayText(plr, 'Say ".e [sequence] [frame] [speed] [freezeFrame]" for more control\n');
	}
}

HookReturnCode ClientSay( SayParameters@ pParams )
{
	CBasePlayer@ plr = pParams.GetPlayer();
	const CCommand@ args = pParams.GetArguments();
	if (args.ArgC() > 0 and (args[0] == ".emote" or args[0] == '.emote_list' or args[0] == '.e'))
	{
		doEmoteCommand(plr, args);
		pParams.ShouldHide = true;
		return HOOK_HANDLED;
	}
	return HOOK_CONTINUE;
}