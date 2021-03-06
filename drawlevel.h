void drawlevel(plevel level,Sint32 mx,Sint32 my,Sint32 dx,Sint32 dy)
{
	int l,x,y;
	Sint32 minx=-dx;
	Sint32 maxx=+dx;
	Sint32 miny=-dy;
	Sint32 maxy=+dy;
	int startx = minx+mx >> SP_ACCURACY+1;
	if (startx < 0)
		startx = 0;
	int endx = maxx+mx >> SP_ACCURACY+1;
	if (endx > level->width)
		endx = level->width;
	int starty = -maxy+my >> SP_ACCURACY+1;
	if (starty < 0)
		starty = 0;
	int endy = -miny+my >> SP_ACCURACY+1;
	if (endy > level->height)
		endy = level->height;
	for (x=startx;x<endx;x++)
	{
		for (y=starty;y<endy;y++)
		{
			for (l=0;l<3;l++)
			{
				psymbol now=level->symbollist[level->layer[l][x+y*level->width]];
				while (now!=NULL)
				{
					if (now->mesh!=NULL)
					{
						if ((now->functionmask & 2)!=2 || now->needed_level<=levelcount)
							spMesh3DwithPos(((2*x)<<SP_ACCURACY)-mx,((-2*y)<<SP_ACCURACY)+my,(l-1)<<SP_ACCURACY,now->mesh,0);
					}
					else
					if (now->functionmask & 1)
					{
						if (show_snow)
						{
							spBindTexture(flake);
							spSetAlphaTest(1);
							Sint32 matrix[16];
							memcpy( matrix, spGetMatrix(), 16 * sizeof( Sint32 ) ); //glPush()
							spTranslate(((2*x)<<SP_ACCURACY)-mx,((-2*y)<<SP_ACCURACY)+my,(l-1)<<(SP_ACCURACY+1));
							spRotateY(w);
							spQuadTex3D(-3 << SP_ACCURACY-2, 3 << SP_ACCURACY-2,0, 0, 0,
													-3 << SP_ACCURACY-2,-3 << SP_ACCURACY-2,0, 0,63,
													 3 << SP_ACCURACY-2,-3 << SP_ACCURACY-2,0,63,63,
													 3 << SP_ACCURACY-2, 3 << SP_ACCURACY-2,0,63, 0,65535);
							spQuadTex3D( 3 << SP_ACCURACY-2, 3 << SP_ACCURACY-2,0,63, 0,
													 3 << SP_ACCURACY-2,-3 << SP_ACCURACY-2,0,63,63,
													-3 << SP_ACCURACY-2,-3 << SP_ACCURACY-2,0, 0,63,
													-3 << SP_ACCURACY-2, 3 << SP_ACCURACY-2,0, 0, 0,65535);
							memcpy( spGetMatrix(), matrix, 16 * sizeof( Sint32 ) ); //glPop()
							spSetAlphaTest(0);
						}
						else
						{
							int c=abs(spSin(w)>>(SP_ACCURACY-7))+127;
							if (c>255)
								c=255;
							spEllipse3D(((2*x)<<SP_ACCURACY)-mx,((-2*y)<<SP_ACCURACY)+my,(l-1)<<(SP_ACCURACY+1),
														abs(spSin(w)*3/4),/*3<<(SP_ACCURACY-2),*/3<<(SP_ACCURACY-2),
														spGetRGB(c,c,c)/*level->symbollist[level->layer[l][x+y*level->width]]->color*/);
						}
					}
					else
					if (now->meshmask & 2)
					{
						Sint32 tx,ty,tz;
						spProjectPoint3D(((2*x)<<SP_ACCURACY)-mx,((-2*y)<<SP_ACCURACY)+my,(l-1)<<(SP_ACCURACY+1),&tx,&ty,&tz,1);
						spSetAlphaTest(1);
						spFontDrawMiddle(tx,ty-font->maxheight/2,tz,now->function,font);
						spSetAlphaTest(0);
					}
					else
					if (now->meshmask & 12)
					{
						if (now->meshmask & 4)
						{
							if (enemyKilled<level->havetokill || now->needed_level>levelcount)
								spBindTexture(door_closed);
							else
								spBindTexture(door_open);
						}
						else
						{
							if (enemyKilled<level->havetokill || now->needed_level>levelcount)
								spBindTexture(door_boss_closed);
							else
								spBindTexture(door_boss_open);
						}
						spSetAlphaTest(1);
						Sint32 matrix[16];
						memcpy( matrix, spGetMatrix(), 16 * sizeof( Sint32 ) ); //glPush()
						spTranslate(((2*x)<<SP_ACCURACY)-mx,((-2*y)<<SP_ACCURACY)+my,((l-1)*4-1<<SP_ACCURACY-1));
						spQuadTex3D(-2 << SP_ACCURACY-1, 6 << SP_ACCURACY-1,0, 0, 0,
									-2 << SP_ACCURACY-1,-2 << SP_ACCURACY-1,0, 0,95,
									 2 << SP_ACCURACY-1,-2 << SP_ACCURACY-1,0,63,95,
									 2 << SP_ACCURACY-1, 6 << SP_ACCURACY-1,0,63, 0,65535);
						memcpy( spGetMatrix(), matrix, 16 * sizeof( Sint32 ) ); //glPop()
						spSetAlphaTest(0);
					}		
					now=now->next;
				}
			}
		}
	}
}

void drawclouds(Sint32 mx,Sint32 my,Sint32 dx,Sint32 dy)
{
	int i;
	for (i=0;i<cloudcount;i++)
		spBlit3D(cloudx[i]-mx,-cloudy[i]+my,cloudz[i],cloud[clouds[i]]);
}
