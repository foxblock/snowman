void drawenemies(Sint32 x,Sint32 y,Sint32 dx,Sint32 dy)
{
  Sint32 minx=-dx;
  Sint32 maxx=+dx;
  Sint32 miny=-dy;
  Sint32 maxy=+dy;
 
  penemy enemy = level->firstenemy;
  while (enemy!=NULL)
  {
    if (enemy->x-x < minx ||
        enemy->x-x > maxx ||
        y-enemy->y < miny ||
        y-enemy->y > maxy)
    {
     enemy=enemy->next;
     continue; 
    }
    if (enemy->symbol->mesh!=NULL)
    {
      //enemymesh
    }
    else
    if ((enemy->symbol->meshmask & 1) == 1) //badcoin
    {
      int c=abs(spSin(w*4)>>(SP_ACCURACY-7))+127;
      if (c>255)
				c=255;
      int r=( enemy->symbol->color>>11     )*8;
      int g=((enemy->symbol->color>>5) & 63)*4;
      int b=( enemy->symbol->color     & 31)*8;
      spEllipse3D(enemy->x-x,y-enemy->y,0,(abs(spSin(w*4))>>SP_HALF_ACCURACY)*(enemy->symbol->measures[2]>>SP_HALF_ACCURACY),enemy->symbol->measures[3],
                    spGetRGB((r*c)>>8,(g*c)>>8,(b*c)>>8));
      Sint32 to=enemy->x-x-enemy->symbol->measures[2]+(2*enemy->symbol->measures[2])*enemy->health/enemy->maxhealth;
      spQuad3D(enemy->x-x-enemy->symbol->measures[2],y-enemy->y+enemy->symbol->measures[3]+(3<<(SP_ACCURACY-5)),0,
               enemy->x-x-enemy->symbol->measures[2],y-enemy->y+enemy->symbol->measures[3]-(3<<(SP_ACCURACY-5)),0,
                                                  to,y-enemy->y+enemy->symbol->measures[3]-(3<<(SP_ACCURACY-5)),0,
                                                  to,y-enemy->y+enemy->symbol->measures[3]+(3<<(SP_ACCURACY-5)),0,enemy->symbol->color);
    }
    else
    if ((enemy->symbol->meshmask & 16)) //enemy with sprite
    {
			Sint32 matrix[16];
			memcpy( matrix, spGetMatrix(), 16 * sizeof( Sint32 ) ); //glPush()
			spBindTexture(enemySur[enemy->symbol->enemy_kind]);
			spTranslate(enemy->x-x,y-enemy->y,0);
			spRotateZ(-enemy->x>>1);
			spQuadTex3D(enemy->symbol->measures[0],enemy->symbol->measures[3],0,  0,  0,
									enemy->symbol->measures[0],enemy->symbol->measures[1],0,  0,127,
									enemy->symbol->measures[2],enemy->symbol->measures[1],0,127,127,
									enemy->symbol->measures[2],enemy->symbol->measures[3],0,127,  0,65535);
      memcpy( spGetMatrix(), matrix, 16 * sizeof( Sint32 ) ); //glPop()
      Sint32 to=enemy->x-x-enemy->symbol->measures[2]+(2*enemy->symbol->measures[2])*enemy->health/enemy->maxhealth;
      spQuad3D(enemy->x-x-enemy->symbol->measures[2],y-enemy->y+enemy->symbol->measures[3]+(3<<(SP_ACCURACY-5)),0,
               enemy->x-x-enemy->symbol->measures[2],y-enemy->y+enemy->symbol->measures[3]-(3<<(SP_ACCURACY-5)),0,
                                                  to,y-enemy->y+enemy->symbol->measures[3]-(3<<(SP_ACCURACY-5)),0,
                                                  to,y-enemy->y+enemy->symbol->measures[3]+(3<<(SP_ACCURACY-5)),0,enemy->symbol->color);
    }    
    enemy=enemy->next; 
  }
}

void moveenemies()
{
  penemy enemy = level->firstenemy;
  while (enemy!=NULL)
  {
    enemy->x+=enemy->dx;
    enemy->y+=enemy->dy;
    if ((enemy->symbol->functionmask & 256) == 256) //waywalker
    {
      int bx  =((((enemy->x                           ) >> (SP_ACCURACY))+1)>>1);
      int bxl =((((enemy->x-enemy->symbol->measures[2]) >> (SP_ACCURACY))+1)>>1);
      int bxr =((((enemy->x+enemy->symbol->measures[2]) >> (SP_ACCURACY))+1)>>1);
      int by  =((((enemy->y+enemy->symbol->measures[3]) >> (SP_ACCURACY))+1)>>1)-1;
      if (bx>=level->width || bx<0 || 
         (enemy->dx<0 && level->symbollist[level->layer[1][bxl+ by   *level->width]]!=NULL && level->symbollist[level->layer[1][bxl+by*level->width]]->form>0) ||
         (enemy->dx>0 && level->symbollist[level->layer[1][bxr+ by   *level->width]]!=NULL && level->symbollist[level->layer[1][bxr+by*level->width]]->form>0) ||
                         level->symbollist[level->layer[1][bx +(by+1)*level->width]]==NULL || 
                         level->symbollist[level->layer[1][bx +(by+1)*level->width]]->form == 0)
        enemy->dx=-enemy->dx;
    }
    enemy=enemy->next;
  }
}
