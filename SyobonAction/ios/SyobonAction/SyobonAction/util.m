#include <Foundation/Foundation.h>
#include "SDL_filesystem.h"

const char *
IOS_BasePath()
{
   static char buf[4096] = "";

   if (buf[0] == '\0')
   {
      char *p = SDL_GetBasePath();
      if (p != NULL)
      {
         strcpy(buf, p);
         free(p);
      }
   }

   return buf;
}

const char *
IOS_SavePath()
{
   static char buf[4096] = "";

   if (buf[0] == '\0')
   {
      NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
      NSString *documentsDirectory = [[paths objectAtIndex:0] stringByAppendingString:@"/"];
      strcpy(buf, [documentsDirectory UTF8String]);
   }

   return buf;
}
