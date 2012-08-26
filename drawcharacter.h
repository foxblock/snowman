char removesnow(int count)
{
  if (ballcount==1)
    return 1;
  if (ballcount==2)
  {
    ballsize[1]-=count<<(ACCURACY-5);
    gotchasmall=500;
    valuesmall=-count;    
    if (ballsize[1]==0)
      ballcount--;
    else
    if (ballsize[1]<0)
      return 1;
  }
  if (ballcount==3)
  {
    ballsize[0]-=count<<(ACCURACY-5);
    if (ballsize[0]<0)
    {
      ballcount--;
      gotchabig=500;
      valuebig=-count-(ballsize[0]>>(ACCURACY-5));
      ballsize[1]+=ballsize[0];
      ballsize[0]=0; 
      gotchasmall=500;
      valuesmall=-count-valuebig;
    }
    else
    {
      gotchabig=500;
      valuebig=-count;
    }
  }
  return 0;
}

char addonesnow()
{
  if (ballcount==3)
  {
    if (ballsize[0]>=(13<<(ACCURACY-4)))
      return 0;
    ballsize[0]+=(1<<(ACCURACY-5));
    gotchabig=500;
    valuebig=1;
    Mix_PlayChannel(-1,hu_chunk,0);
    return 1;
  }
  if (ballcount==1)
  {
    //ballsize[2]+=(1<<(ACCURACY-5));
    //if (ballsize[2]>=(7<<(ACCURACY-4)))
    {
      ballsize[1]=0;
      ballcount++;
    }
  }
  if (ballcount==2)
  {
    ballsize[1]+=(1<<(ACCURACY-5));
    gotchasmall=500;
    valuesmall=1;
    if (ballsize[1]>=(9<<(ACCURACY-4)))
    {
      ballsize[0]=0;
      ballcount++;
    }
    Mix_PlayChannel(-1,hu_chunk,0);
    return 1;
  }
  return 0;
}


void drawcharacter(Sint32 x,Sint32 y,Sint32 z,char right)
{
  Sint32* modellViewMatrix=engineGetModellViewMatrix();
  int i;
  int red=((mycos(damaged*(1<<(ACCURACY-6)))*127)>>ACCURACY)+128;
  for (i=3-ballcount;i<3;i++)
  {
    y+=ballsize[i];
    Sint32 r=ballsize[i]+(1<<(ACCURACY-5));
    //engineEllipse(x,y,z,r,r,getRGB(255,255,255));
    Sint32 matrix[16];    
    memcpy(matrix,modellViewMatrix,64);
    modellViewMatrix[12]+=x;
    modellViewMatrix[13]+=y;
    modellViewMatrix[14]+=z;    
    if (i==0)
      engineRotate(0,0,1<<ACCURACY,angle);
    if (i==1)
    {
      if (ballcount==3)
        engineRotate(0,0,-1<<ACCURACY,angle);
      else
        engineRotate(0,0, 1<<ACCURACY,angle);
    }
    drawMeshXYZS(0,0,0,r,sphere,getRGB(255,red,red));
    memcpy(modellViewMatrix,matrix,64);
    if (i==2)
    {
      if (right)
      {
        drawMeshXYZS(x,y,z,r,sphere_nose,getRGB(255,127,0));
        engineEllipse(x+(1<<(ACCURACY-3)),y+(1<<(ACCURACY-2)),z+r,1<<(ACCURACY-2),1<<(ACCURACY-2),getRGB(0,0,0));
      }
      else
      {
        drawMeshXYZS(x,y,z,-r,sphere_nose,getRGB(255,127,0));
        engineEllipse(x-(1<<(ACCURACY-3)),y+(1<<(ACCURACY-2)),z+r,1<<(ACCURACY-2),1<<(ACCURACY-2),getRGB(0,0,0));
      }
    }
    y+=ballsize[i];
  }
  if (broom_exist)
  {
    int sum=0;
    int i;
    for (i=3-ballcount;i<3;i++)
      sum+=ballsize[i]*2;
    Sint32 matrix[16];
    memcpy(matrix,modellViewMatrix,64);
    modellViewMatrix[12]+=x;
    modellViewMatrix[13]+=y-(sum>>1);
    modellViewMatrix[14]+=z+(1<<ACCURACY);    
    if (right)
    {
      engineRotate(0,0, 1<<ACCURACY, MY_PI>>2);
      if (in_hit>192)
        engineRotate(0<<ACCURACY,0<<ACCURACY,-1<<ACCURACY,((288-in_hit)*MY_PI)>>7);
      else
        engineRotate(0<<ACCURACY,0<<ACCURACY,-1<<ACCURACY,(in_hit*MY_PI)>>8);
    }
    else
    {
      engineRotate(0,0, 1<<ACCURACY, 7*MY_PI>>2);
      if (in_hit>192)
        engineRotate(0<<ACCURACY,0<<ACCURACY, 1<<ACCURACY,((288-in_hit)*MY_PI)>>7);
      else
        engineRotate(0<<ACCURACY,0<<ACCURACY, 1<<ACCURACY,(in_hit*MY_PI)>>8);
    }
    
    if (in_hit>192)
      drawMeshXYZS(0,(288-in_hit)<<(ACCURACY-8),0,(1<<ACCURACY),broom,getRGB(86,22,0));
    else
      drawMeshXYZS(0,(    in_hit)<<(ACCURACY-7),0,(1<<ACCURACY),broom,getRGB(86,22,0));
    memcpy(modellViewMatrix,matrix,64);
  }
}

char testX(Sint32 x,Sint32 ox)
{
  biggest=2;
  if (ballcount>2)
  {
    if (ballsize[0]>ballsize[2] && ballsize[0]>ballsize[1])
      biggest=0;
    if (ballsize[1]>ballsize[2] && ballsize[1]>ballsize[0])
      biggest=1;
  }
  else
  if (ballcount>1)
  {
    if (ballsize[1]>ballsize[2])
      biggest=1;
  }
  bx =((x>>(ACCURACY))+1)>>1;
  bxl=(((x-ballsize[biggest])>>(ACCURACY))+1)>>1;
  bxr=(((x+ballsize[biggest])>>(ACCURACY))+1)>>1;
  by =((y>>(ACCURACY))+1)>>1;
  byb=by;
  if (((2*by-1)<<ACCURACY)!=y)
    byb+=1;
  int sum=0;
  int i;
  for (i=3-ballcount;i<3;i++)
    sum+=ballsize[i]*2;
  byt =((((y-sum)>>(ACCURACY))+1)>>1);
  bym =(byt + byb)>>1;

  //printf("b: %i m: %i t: %i xl: %i x: %i xr: %i\n",byb,bym,byt,bxl,bx,bxr);
  //Solid Block on the left?
  if (bxl>=0 && bxl<level->width)
  {
    if (byb>0 && level->symbollist[level->layer[1][bxl+(byb-1)*level->width]]       != NULL &&
                 level->symbollist[level->layer[1][bxl+(byb-1)*level->width]]->form > 0)
    {
      if ((level->symbollist[level->layer[1][bxl+(byb-1)*level->width]]->functionmask & 4) == 4 && ox>x) //left
        return 0; 
      if ((level->symbollist[level->layer[1][bxl+(byb-1)*level->width]]->functionmask & 8) == 8 && ox<x) //right
        return 0; 
      return 1;
    }
    if (byt>=0 && level->symbollist[level->layer[1][bxl+(byt)*level->width]]       != NULL &&
                  level->symbollist[level->layer[1][bxl+(byt)*level->width]]->form > 0)
    {
      if ((level->symbollist[level->layer[1][bxl+(byt)*level->width]]->functionmask & 4) == 4 && ox>x) //left
        return 0; 
      if ((level->symbollist[level->layer[1][bxl+(byt)*level->width]]->functionmask & 8) == 8 && ox<x) //right
        return 0; 
      return 1;
    }
    if (bym>=0 && level->symbollist[level->layer[1][bxl+(bym)*level->width]]       != NULL &&
                  level->symbollist[level->layer[1][bxl+(bym)*level->width]]->form > 0)
    {
      if ((level->symbollist[level->layer[1][bxl+(bym)*level->width]]->functionmask & 4) == 4 && ox>x) //left
        return 0; 
      if ((level->symbollist[level->layer[1][bxl+(bym)*level->width]]->functionmask & 8) == 8 && ox<x) //right
        return 0; 
      return 1;
    }
  }
  //Solid Block on the right?
  if (bxr>=0 && bxr<level->width)
  {
    if (byb>0 && level->symbollist[level->layer[1][bxr+(byb-1)*level->width]]       != NULL &&
                 level->symbollist[level->layer[1][bxr+(byb-1)*level->width]]->form > 0)
    {
      if ((level->symbollist[level->layer[1][bxr+(byb-1)*level->width]]->functionmask & 4) == 4 && ox>x) //left
        return 0; 
      if ((level->symbollist[level->layer[1][bxr+(byb-1)*level->width]]->functionmask & 8) == 8 && ox<x) //right
        return 0; 
      return 2;
    }
    if (byt>=0 && level->symbollist[level->layer[1][bxr+(byt)*level->width]]       != NULL &&
                  level->symbollist[level->layer[1][bxr+(byt)*level->width]]->form > 0)
    {
      if ((level->symbollist[level->layer[1][bxr+(byt)*level->width]]->functionmask & 4) == 4 && ox>x) //left
        return 0; 
      if ((level->symbollist[level->layer[1][bxr+(byt)*level->width]]->functionmask & 8) == 8 && ox<x) //right
        return 0; 
      return 2;
    }
    if (bym>=0 && level->symbollist[level->layer[1][bxr+(bym)*level->width]]       != NULL &&
                  level->symbollist[level->layer[1][bxr+(bym)*level->width]]->form > 0)
    {
      if ((level->symbollist[level->layer[1][bxr+(bym)*level->width]]->functionmask & 4) == 4 && ox>x) //left
        return 0; 
      if ((level->symbollist[level->layer[1][bxr+(bym)*level->width]]->functionmask & 8) == 8 && ox<x) //right
        return 0; 
      return 2;
    }
  }
  //Solid Block where I am?
  /*if (bx>=0 && bx<level->width &&
     ((byb>0 && level->symbollist[level->layer[1][bx +(byb-1)*level->width]]       != NULL &&
                 level->symbollist[level->layer[1][bx +(byb-1)*level->width]]->form > 0) ||
      (byt>=0 && level->symbollist[level->layer[1][bx +(byt  )*level->width]]       != NULL &&
                 level->symbollist[level->layer[1][bx +(byt  )*level->width]]->form > 0) ||
      (bym>=0 && level->symbollist[level->layer[1][bx +(bym  )*level->width]]       != NULL &&
                 level->symbollist[level->layer[1][bx +(bym  )*level->width]]->form > 0)))
    return 1;*/
  return 0;
}

char testX2(Sint32 x,Sint32 ox)
{
  biggest=2;
  if (ballcount>2)
  {
    if (ballsize[0]>ballsize[2] && ballsize[0]>ballsize[1])
      biggest=0;
    if (ballsize[1]>ballsize[2] && ballsize[1]>ballsize[0])
      biggest=1;
  }
  else
  if (ballcount>1)
  {
    if (ballsize[1]>ballsize[2])
      biggest=1;
  }
  int sum=0;
  int i;
  for (i=3-ballcount;i<3;i++)
    sum+=ballsize[i]*2;
  bx =((x>>(ACCURACY))+1)>>1;
  bxl=(((x-ballsize[biggest])>>(ACCURACY))+1)>>1;
  bxr=(((x+ballsize[biggest])>>(ACCURACY))+1)>>1;
  /*by =((y>>(ACCURACY))+1)>>1;
  byb=by;
  if (((2*by-1)<<ACCURACY)!=y)
    byb+=1;
  byt =((((y-sum)>>(ACCURACY))+1)>>1);
  bym =(byt + byb)>>1;*/
  bym = (((y-sum/2)>>ACCURACY)+1)/2;
  //printf("b: %i m: %i t: %i xl: %i x: %i xr: %i\n",byb,bym,byt,bxl,bx,bxr);
  //Solid Block on the left?
  if (bxl>=0 && bxl<level->width)
  {
    if (bym>=0 && level->symbollist[level->layer[1][bxl+(bym)*level->width]]       != NULL &&
                  level->symbollist[level->layer[1][bxl+(bym)*level->width]]->form > 0)
    {
      if ((level->symbollist[level->layer[1][bxl+(bym)*level->width]]->functionmask & 4) == 4 && ox>x) //left
        return 0; 
      if ((level->symbollist[level->layer[1][bxl+(bym)*level->width]]->functionmask & 8) == 8 && ox<x) //right
        return 0; 
      return 1;
    }
  }
  //Solid Block on the right?
  if (bxr>=0 && bxr<level->width)
  {
    if (bym>=0 && level->symbollist[level->layer[1][bxr+(bym)*level->width]]       != NULL &&
                  level->symbollist[level->layer[1][bxr+(bym)*level->width]]->form > 0)
    {
      if ((level->symbollist[level->layer[1][bxr+(bym)*level->width]]->functionmask & 4) == 4 && ox>x) //left
        return 0; 
      if ((level->symbollist[level->layer[1][bxr+(bym)*level->width]]->functionmask & 8) == 8 && ox<x) //right
        return 0; 
      return 2;
    }
  }
  return 0;
}

char fattest(Sint32 *x,Sint32 ox)
{
  char i=testX2((*x),ox);
  if (i==1)
  {
    while (i==1)
    {
      i=testX2((*x),ox);
      (*x)+=(1<<(ACCURACY-4));
      printf("Left\n");
    }
    return 1;
  }
  if (i==2)
  {
    while (i==2)
    {
      i=testX2((*x),ox);
      (*x)-=(1<<(ACCURACY-4));
      printf("Right\n");
    }
    return 1;
  }
  return 0;
}

void playerEnemyInteraction()
{
  if (damaged)
    return;
  int sum=0;
  int i;
  for (i=3-ballcount;i<3;i++)
    sum+=ballsize[i]*2;
  biggest=2;
  if (ballcount>2)
  {
    if (ballsize[0]>ballsize[2] && ballsize[0]>ballsize[1])
      biggest=0;
    if (ballsize[1]>ballsize[2] && ballsize[1]>ballsize[0])
      biggest=1;
  }
  else
  if (ballcount>1)
  {
    if (ballsize[1]>ballsize[2])
      biggest=1;
  }
  penemy enemy=level->firstenemy;
  while (enemy!=NULL)
  {
    //Distance
    if ( x+ballsize[biggest] >= enemy->x-enemy->symbol->measures[2] &&
         x-ballsize[biggest] <= enemy->x+enemy->symbol->measures[2] &&
         y-sum               <= enemy->y+enemy->symbol->measures[3] &&
         y                   >= enemy->y-enemy->symbol->measures[3] ) //Hit
    {
      damaged=MY_PI>>(ACCURACY-8);
      if (removesnow(3))
      {
        fade2=1024;
        Mix_PlayChannel(-1,negative_chunk,0);
        return;
      }
      return;
    }
    enemy=enemy->next;
  }
}

void broomEnemyInteraction(char right)
{
  int sum=0;
  int i;
  for (i=3-ballcount;i<3;i++)
    sum+=ballsize[i]*2;
  Sint32 broomx=x;
  if (right)
    broomx+=+(2<<ACCURACY);
  else
    broomx-=+(2<<ACCURACY);
  Sint32 broomy=y-(sum>>1)+(1<<ACCURACY);
  penemy ebefore=NULL;
  penemy enemy=level->firstenemy;
  while (enemy!=NULL)
  {
    //Distance
    if ( broomx >= enemy->x-enemy->symbol->measures[2]-(1<<ACCURACY) &&
         broomx <= enemy->x+enemy->symbol->measures[2]+(1<<ACCURACY) &&
         broomy >= enemy->y-enemy->symbol->measures[3]-(1<<ACCURACY) &&
         broomy <= enemy->y+enemy->symbol->measures[3]+(1<<ACCURACY) ) //Hit
    {
      //newexplosion(PARTICLES,broomx,broomy,0,1024,getRGB(86,22,0));
      enemy->health-=2;
      newexplosion(2*PARTICLES,enemy->x,enemy->y,0,1024,enemy->symbol->color);
      if (enemy->health<=0)
      {
        enemyKilled++;
        if (ebefore==NULL)
          level->firstenemy=enemy->next;
        else
          ebefore->next=enemy->next;
        //newexplosion(PARTICLES,enemy->x,enemy->y,0,1024,enemy->symbol->color);
        free(enemy);
        enemy=ebefore;
      }
      break;
    }
    ebefore=enemy;
    if (enemy!=NULL)
      enemy=enemy->next;
  }
}

void savelevelcount()
{
  SDL_RWops *file=SDL_RWFromFile("./levelcount.dat","wb");
  levelcount^=1337;
  SDL_RWwrite(file,&levelcount,sizeof(int),1);
  SDL_RWwrite(file,&volume,sizeof(int),1);
  SDL_RWwrite(file,&volumefactor,sizeof(int),1);
  levelcount^=1337;
  SDL_RWclose(file);
}

void loadlevelcount()
{
  SDL_RWops *file=SDL_RWFromFile("./levelcount.dat","rb");
  if (file==NULL)
  {
    //Compatibility
    file=SDL_RWFromFile("./data/levelcount.dat","rb");
    if (file==NULL)
      return;
  }
  SDL_RWread(file,&levelcount,sizeof(int),1);
  SDL_RWread(file,&volume,sizeof(int),1);
  SDL_RWread(file,&volumefactor,sizeof(int),1);
  levelcount^=1337;
  SDL_RWclose(file);
}
