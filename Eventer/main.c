//
//  See http://developer.apple.com/library/mac/#documentation/Darwin/Conceptual/FSEvents_ProgGuide/UsingtheFSEventsFramework/UsingtheFSEventsFramework.html
//
//  main.c
//  Eventer
//
//  Created by Manuel Carrasco Molina on 29.11.11.
//  Copyright (c) 2011 Pomcast. All rights reserved.
//

#include <CoreFoundation/CoreFoundation.h>
#include <CoreServices/CoreServices.h>

void mycallback(ConstFSEventStreamRef streamRef,
                void *clientCallBackInfo,
                size_t numEvents,
                void *eventPaths,
                const FSEventStreamEventFlags eventFlags[],
                const FSEventStreamEventId eventIds[]);

int main (int argc, const char * argv[])
{
    /* Define variables and create a CFArray object containing
     CFString objects containing paths to watch.
     */
    CFStringRef mypath = CFSTR("/");
    CFArrayRef pathsToWatch = CFArrayCreate(NULL, (const void **)&mypath, 1, NULL);
    void *callbackInfo = NULL; // could put stream-specific data here.
    FSEventStreamRef stream;
    CFAbsoluteTime latency = 3.0; /* Latency in seconds */
    
    /* Create the stream, passing in a callback */
    stream = FSEventStreamCreate(NULL,
                                 &mycallback,
                                 callbackInfo,
                                 pathsToWatch,
                                 kFSEventStreamEventIdSinceNow, /* Or a previous event ID */
                                 latency,
                                 kFSEventStreamCreateFlagNone /* Flags explained in reference */
                                 );
    FSEventStreamScheduleWithRunLoop(stream, 
                                     CFRunLoopGetCurrent(),         
                                     kCFRunLoopDefaultMode);
    FSEventStreamStart(stream);
    CFShow(CFSTR("Starting the Run Loop\n"));
    CFRunLoopRun();
    return 0;
}

void mycallback(
                ConstFSEventStreamRef streamRef,
                void *clientCallBackInfo,
                size_t numEvents,
                void *eventPaths,
                const FSEventStreamEventFlags eventFlags[],
                const FSEventStreamEventId eventIds[])
{
    int i;
    char **paths = eventPaths;
    
    // printf("Callback called\n");
    for (i=0; i<numEvents; i++) {
        /* flags are unsigned long, IDs are uint64_t */
        printf("Change %llu in %s, flags %u\n", eventIds[i], paths[i], eventFlags[i]);
    }
}

