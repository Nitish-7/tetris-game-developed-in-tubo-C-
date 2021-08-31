#include<iostream.h>
#include<conio.h>
#include<graphics.h>
#include<stdlib.h>
#include<stdio.h>
#include<dos.h>
#include<time.h>

//global var
int blocktype,gameended,
    barx[4],bary[4]; //position of each bar x&y

//container measure
int  minx=180,
     miny=10,
     maxx=460,
     maxy=465,
     widthx=maxx-minx,
     widthy=maxy-miny;

int  container[11][18];
int  collhight[11], maxcollhight=5+(25*17) ,x,y;     //container x & y

// characters
char ch;

// for container image
long double imagesz;
void *blocksimage;

// ALL functions declaration
void blockdownmover();
void blocksidemover();
void blockrotator();
void blockmaker();
void blockremover();
void instantiate();
void changecollhight();
void checkkeydown();
void checkscoreline();
void deleteline();
 int iscollided();
 int gameover();
 int canrotate();

int gameover()
{
  if(gameended==1)
   return 1;
  else
   return 0;
}

void checkkeydown()
{
  if(ch=='x'||ch=='X')
    gameended=1;
  else if(ch=='M'||ch=='K')
   {
     for(int i=0;i<4;i++)
      {
       if(ch=='M'&& container[(barx[i]/25)+1][bary[i]/25]==1)
	 return;
       else if(ch=='K'&& container[(barx[i]/25)-1][bary[i]/25]==1)
	 return;
      }
     blocksidemover();
   }
  else if(ch=='H'||ch=='P')
   {
     if(canrotate())
      blockrotator();
   }
}

void deleteline()
{
 for(int J=y;J>=maxcollhight/25;J--)
  {
   for(int I=0;I<11;I++)
    container[I][J]=container[I][J-1];
  }

 //line removing
 imagesz=imagesize(5,5+(25*y),275,5+((y+1)*25));
 blocksimage=malloc(imagesz);
 getimage(5,5+(25*y),275,((y+1)*25),blocksimage);
 delay(500);
 putimage(5,5+(25*y),blocksimage,1);

 free(blocksimage);
 imagesz=0;

 //changing each collhight
 for(int i=0;i<11;i++)
  collhight[i]+=25;
 maxcollhight+=25;

 imagesz=imagesize(5,maxcollhight,275,(y*25));
 blocksimage=malloc(imagesz);
 getimage(5,maxcollhight,275,(y*25),blocksimage);
 delay(500);
 putimage(5,maxcollhight,blocksimage,1);
 putimage(5,maxcollhight+25,blocksimage,1);

 free(blocksimage);
 imagesz=0;

}

void checkscoreline()
{
  int countbars;
  for(int i=0;i<4;i++)
  {
   x=barx[i]/25; y=bary[i]/25;
   container[x][y]=1;
  }
  for(y=17;y>=maxcollhight/25;y--)
  {
    countbars=0;
    for(x=0;x<11;x++)
     {
      if(container[x][y]==0)
       break;
      else
       countbars+=1;
     }
    if(countbars==11)
     {
      //cout<<endl<<"deleting line pres button"<<endl;
      //getch();
      deleteline();
      break;
     }
  }
}

void changecollhight()
{
 for(int i=0;i<4;i++)
  {
   int I;
   I=barx[i]/25;
    if(collhight[I]>=bary[i])
     {
      collhight[I]=bary[i]-25;

      //check for gameover or container full
      if(collhight[I]<30)
       {
	 gameended=1; cout<<"collhight["<<I<<"] = "<<collhight[I]+25<<endl;
	 cout<<"bary["<<i<<"] = "<<bary[i]<<endl;
       }

      //giving maxhight in container
      if(maxcollhight>collhight[I])
       {
	 maxcollhight=collhight[I];
	 //cout<<endl<<"collhight["<<I<<"] = "<<collhight[I]<<endl<<"maxcollhight = "<<maxcollhight<<endl;
       }
     }
  }
}
int iscollided()
{
 for(int i=0;i<4;i++)
 {
  int I;
  I=barx[i]/25;
   if(collhight[I]==bary[i])
    {
     changecollhight();
     checkscoreline();
     return 1;
    }
 }
 return 0;
}
int canrotate()
{
 int  I=0,J=0;
 for(int i=0;i<4;i++)
  {
     //if horizotal
     if(bary[i]==bary[2])
      {
	 if(barx[i]<barx[2])
	  J=bary[2]-(barx[2]-barx[i]);

	 else if(barx[i]>barx[2])
	  J=bary[2]+(barx[i]-barx[2]);

	 I=barx[2];

	 if(container[I/25][J/25]==1)
	  return 0;
      }
     //if dvertical
     else if(barx[i]==barx[2])
      {
	 if(bary[i]<bary[2])
	  I=barx[2]+(bary[2]-bary[i]);

	 else if(bary[i]>bary[2])
	  I=barx[2]-(bary[i]-bary[2]);

	 J=bary[2];

	 if(container[I/25][J/25]==1)
	  return 0;
      }
     //if diagonal
     else
      {
	 I=barx[i]; J=bary[i];

	 if(barx[i]<barx[2] && bary[i]<bary[2])
	  I=barx[2]+(barx[2]-barx[i]);
	 else if(barx[i]<barx[2] && bary[i]>bary[2])
	  J=bary[2]-(bary[i]-bary[2]);
	 else if(barx[i]>barx[2] && bary[i]<bary[2])
	  J=bary[2]+(bary[2]-bary[i]);
	 else if(barx[i]>barx[2] && bary[i]>bary[2])
	  I=barx[2]-(barx[i]-barx[2]);

	 if(container[I/25][J/25]==1)
	  return 0;
      }
  }
  return 1;

}

void blockrotator()
{

 if(blocktype<4)
  {
    //clearing old postion
    blockremover();

   for(int i=0;i<4;i++)
    {
     //if horizotal
     if(bary[i]==bary[2])
      {
	 if(barx[i]<barx[2])
	  bary[i]=bary[2]-(barx[2]-barx[i]);

	 else if(barx[i]>barx[2])
	  bary[i]=bary[2]+(barx[i]-barx[2]);

	 barx[i]=barx[2];
      }
     //if vertical
     else if(barx[i]==barx[2])
      {
	 if(bary[i]<bary[2])
	  barx[i]=barx[2]+(bary[2]-bary[i]);

	 else if(bary[i]>bary[2])
	  barx[i]=barx[2]-(bary[i]-bary[2]);

	 bary[i]=bary[2];
      }
     //if diagonal
     else
      {
	 if(barx[i]<barx[2] && bary[i]<bary[2])
	  barx[i]=barx[2]+(barx[2]-barx[i]);
	 else if(barx[i]<barx[2] && bary[i]>bary[2])
	  bary[i]=bary[2]-(bary[i]-bary[2]);
	 else if(barx[i]>barx[2] && bary[i]<bary[2])
	  bary[i]=bary[2]+(bary[2]-bary[i]);
	 else if(barx[i]>barx[2] && bary[i]>bary[2])
	  barx[i]=barx[2]-(barx[i]-barx[2]);
      }
    }

     // checking if out of container
     int ifleftout=0,ifrightout=0,ifdownout=0,maxout=0;
     for(i=0;i<4;i++)
      {
       if(barx[i]>(5+(25*10)))
	{
	 ifrightout=1;
	  if(maxout<barx[i]-(5+(25*10)))
	   maxout=barx[i]-(5+(25*10));
	}
       else if(barx[i]<(5+(25*0)))
	{
	 ifleftout=1;
	  if(maxout<(5+(25*0))-barx[i])
	   maxout=(5+(25*0))-barx[i];
	}
       else if(bary[i]>(5+(25*17)))
	{
	 ifdownout=1;
	  if(maxout<barx[i]-(5+(25*17)))
	   maxout=barx[i]-(5+(25*17));
	}

      }

     //fixing position of blocks
     for(i=0;i<4;i++)
     {
       if(ifleftout==1)
	barx[i]+=maxout;
       else if(ifrightout==1)
	barx[i]-=maxout;
       else if(ifdownout==1)
	bary[i]-=maxout;
     }

    //making new rotated blocks
    blockmaker();
  }

}
void blocksidemover()
{

  if(ch=='M' && barx[0]<5+(25*10) && barx[1]<5+(25*10) && barx[2]<5+(25*10) && barx[3]<5+(25*10))
   {
     blockremover();
    for(int i=0;i<4;i++)
     {
      barx[i]+=25;
     }
     blockmaker();
   }

  else if(ch=='K' && barx[0]>5+(25*0) && barx[1]>5+(25*0) && barx[2]>5+(25*0) && barx[3]>5+(25*0))
   {
     blockremover();
    for(int i=0;i<4;i++)
     {
      barx[i]-=25;
     }
     blockmaker();
   }
  ch=NULL;
}

void blockdownmover()
{
  if(iscollided())
    instantiate();

  if(gameended!=1)
   {
    blockremover();
     for(int i=0;i<4;i++)
      bary[i]+=25;
    blockmaker();
   }
}

void blockmaker()
{
  for(int i=0;i<4;i++)
   bar(barx[i],bary[i],barx[i]+20,bary[i]+20);
}

void blockremover()
{
  delay(400);
  for(int i=0;i<4;i++)
  {
   setviewport(minx+barx[i],miny+bary[i],minx+barx[i]+20,miny+bary[i]+20,1);
   clearviewport();
  }
  setviewport(minx,miny,maxx,maxy,1);
}

void instantiate()
{
 // middle bar bar(5+(25*5),5,25+(25*5),25);
 int temp;
 //randamise blocktype
 randomize();
 temp=random(100);
  for(;temp>0;temp/=10)
    blocktype+=temp%10;
 blocktype=blocktype%10;

 if(blocktype>4)
  blocktype-=5;

 switch (blocktype)
 {
  case 0 : barx[0]=5+(25*5); barx[1]=5+(25*4); barx[2]=5+(25*5); barx[3]=5+(25*6);
  /*T*/    bary[0]=5+(25*0); bary[1]=5+(25*1); bary[2]=5+(25*1); bary[3]=5+(25*1);
	     for(int i=0;i<4;i++)
	       bar(barx[i],bary[i],barx[i]+20,bary[i]+20);
	   break;

  case 1 : barx[0]=5+(25*4); barx[1]=5+(25*5); barx[2]=5+(25*5); barx[3]=5+(25*6);
  /*Z*/    bary[0]=5+(25*0); bary[1]=5+(25*0); bary[2]=5+(25*1); bary[3]=5+(25*1);
	     for(i=0;i<4;i++)
	       bar(barx[i],bary[i],barx[i]+20,bary[i]+20);
	   break;

  case 2 : barx[0]=5+(25*6); barx[1]=5+(25*4); barx[2]=5+(25*5); barx[3]=5+(25*6);
  /*L*/    bary[0]=5+(25*0); bary[1]=5+(25*1); bary[2]=5+(25*1); bary[3]=5+(25*1);
	     for(i=0;i<4;i++)
	       bar(barx[i],bary[i],barx[i]+20,bary[i]+20);
	   break;

  case 3 : barx[0]=5+(25*4); barx[1]=5+(25*5); barx[2]=5+(25*6); barx[3]=5+(25*7);
  /*I*/    bary[0]=5+(25*0); bary[1]=5+(25*0); bary[2]=5+(25*0); bary[3]=5+(25*0);
	     for(i=0;i<4;i++)
	       bar(barx[i],bary[i],barx[i]+20,bary[i]+20);
	   break;

  case 4 : barx[0]=5+(25*5); barx[1]=5+(25*6); barx[2]=5+(25*5); barx[3]=5+(25*6);
 /*squar*/ bary[0]=5+(25*0); bary[1]=5+(25*0); bary[2]=5+(25*1); bary[3]=5+(25*1);
	     for(i=0;i<4;i++)
	       bar(barx[i],bary[i],barx[i]+20,bary[i]+20);
	   break;
 }

}

void containerview()
{
 // container measurement
 /*int minx=(getmaxx()/4)+20,
     miny=10,
     maxx=((getmaxx()*3)/4)-20,
     maxy=getmaxy()-14,
     widthx=maxx-minx,
     widthy=maxy-miny;*/

 setviewport(minx,miny,maxx,maxy,1);
 rectangle(0,0,widthx,widthy);
/* for(int i=0;i<18;i++)
 {
  bar(5,5+(i*25),25,25+(i*25));
  bar(5+(i*25),5,25+(i*25),25);
 }*/

 //cout<<"width x = "<<widthx<<endl<<"width y = "<<widthy;
 //cout<<endl<<"minx = "<<minx<<endl<<"miny = "<<miny<<endl<<"maxx = "<<maxx<<endl<<"maxy = "<<maxy;
}

void scoreview()
{
 //setviewport(maxx,miny,maxx+constant,maxy,1);
 //rectangle(0,0,viewwidthx,viewwidthy);

 // now display score
 //outxy();

}

void UI()
{
 //container view
 containerview();

 //score and level view
 scoreview();

}

void graphicsinitialise()
{
  /* request auto detection */
  int gdriver = DETECT, gmode, errorcode;

  /* initialize graphics mode */
  initgraph(&gdriver, &gmode, "");

  /* read result of initialization */
  errorcode = graphresult();

  if (errorcode != grOk)  /* an error occurred */
   {
    printf("Graphics error: %s\n", grapherrormsg(errorcode));
    printf("Press any key to halt:");
    getch();
    exit(1);             /* return with error code */
   }

}

void main()
{
 graphicsinitialise();

 //draws UI
 UI();

 for(int i=0;i<11;i++)
   collhight[i]=5+(25*17);

 instantiate();

 while(!gameover())
  {
   if(kbhit())
    {
     ch=getch();
     checkkeydown();
    }

   //block down move
   else if(gameended!=1)
     blockdownmover();
  }

 /* clean up before exit */
 getch();
 closegraph();

}