int intro_pos=19000;

void draw_intro(void)
{
  spClearTarget(12345);
  
  /*drawtextMXMY(engineGetSurface(SURFACE_SURFACE),engineGetWindowX()>>1,(((intro_pos-9400)*(engineGetWindowY()>>6))>>7),"There was the big war.",engineGetSurface(SURFACE_KEYMAP));
  drawtextMXMY(engineGetSurface(SURFACE_SURFACE),engineGetWindowX()>>1,(((intro_pos-8600)*(engineGetWindowY()>>6))>>7),"The evil flat rotating",engineGetSurface(SURFACE_KEYMAP));
  drawtextMXMY(engineGetSurface(SURFACE_SURFACE),engineGetWindowX()>>1,(((intro_pos-7800)*(engineGetWindowY()>>6))>>7),"circles took the power.",engineGetSurface(SURFACE_KEYMAP));
  drawtextMXMY(engineGetSurface(SURFACE_SURFACE),engineGetWindowX()>>1,(((intro_pos-7000)*(engineGetWindowY()>>6))>>7),"But the resistance movement",engineGetSurface(SURFACE_KEYMAP));
  drawtextMXMY(engineGetSurface(SURFACE_SURFACE),engineGetWindowX()>>1,(((intro_pos-6200)*(engineGetWindowY()>>6))>>7),"fought against the intruders.",engineGetSurface(SURFACE_KEYMAP));
  drawtextMXMY(engineGetSurface(SURFACE_SURFACE),engineGetWindowX()>>1,(((intro_pos-5400)*(engineGetWindowY()>>6))>>7),"That was a long time ago.",engineGetSurface(SURFACE_KEYMAP));
  drawtextMXMY(engineGetSurface(SURFACE_SURFACE),engineGetWindowX()>>1,(((intro_pos-4600)*(engineGetWindowY()>>6))>>7),"The movement is exhausted,",engineGetSurface(SURFACE_KEYMAP));
  drawtextMXMY(engineGetSurface(SURFACE_SURFACE),engineGetWindowX()>>1,(((intro_pos-3800)*(engineGetWindowY()>>6))>>7),"but the enemies are still there.",engineGetSurface(SURFACE_KEYMAP));
  drawtextMXMY(engineGetSurface(SURFACE_SURFACE),engineGetWindowX()>>1,(((intro_pos-3000)*(engineGetWindowY()>>6))>>7),"You are the last (snow)man.",engineGetSurface(SURFACE_KEYMAP));
  drawtextMXMY(engineGetSurface(SURFACE_SURFACE),engineGetWindowX()>>1,(((intro_pos-2200)*(engineGetWindowY()>>6))>>7),"Bring it to an end.",engineGetSurface(SURFACE_KEYMAP));*/
  char buffer[256];
	sprintf( buffer, "Snowman is a game about... snowmen!\nfps: %i", spGetFPS() );
	spFontDrawMiddle( screen->w/2, 1, -1, buffer, font );
  spFlip();
}

int calc_intro(Uint32 steps)
{
  PspInput engineInput = spGetInput();
  intro_pos-=steps;
  if (intro_pos<=0)
    return 1;
  if (engineInput->button[SP_BUTTON_LEFT] || engineInput->button[SP_BUTTON_RIGHT] || engineInput->button[SP_BUTTON_DOWN] || engineInput->button[SP_BUTTON_UP])
    return 1;
  
  return 0;
}

void intro()
{
	spSetZTest(0);
	spSetZSet(0);
  spLoop(draw_intro,calc_intro,10,resize,NULL);
}
