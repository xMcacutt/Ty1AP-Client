#pragma once
#include <string>
#include <map>

struct LevelCollectibleInfo {
    std::string levelName; 
    std::vector<std::pair<std::string, std::string>> rainbowScales; 
    std::vector<std::pair<std::string, std::string>> goldenCogs; 
    std::vector<std::pair<std::string, std::string>> bilbies;
    std::vector<std::pair<std::string, std::string>> pictureFrames; 
    std::vector<std::pair<std::string, std::string>> signposts; 
    std::vector<std::pair<std::string, std::string>> extraLives;

    LevelCollectibleInfo(
        const std::string& levelName,
        const std::vector<std::pair<std::string, std::string>>& rainbowScales,
        const std::vector<std::pair<std::string, std::string>>& goldenCogs,
        const std::vector<std::pair<std::string, std::string>>& bilbies,
        const std::vector<std::pair<std::string, std::string>>& pictureFrames,
        const std::vector<std::pair<std::string, std::string>>& signposts,
        const std::vector<std::pair<std::string, std::string>>& extraLives
    )
        : levelName(levelName),
        rainbowScales(rainbowScales),
        goldenCogs(goldenCogs),
        bilbies(bilbies),
        pictureFrames(pictureFrames),
        signposts(signposts),
        extraLives(extraLives)
    {}
}; 

const LevelCollectibleInfo Z1Info(
    "Rainbow Cliffs",
    { 
        {"Rainbow Scale 1", "Behind right house in Hub 1"},
        {"Rainbow Scale 2", "Behind left house in Hub 1"},
        {"Rainbow Scale 3", "In corner behind Julius' lab"},
        {"Rainbow Scale 4", "Under Thunder Egg collector in Hub 2"},
        {"Rainbow Scale 5", "Just past flame logs in Hub 3"},
        {"Rainbow Scale 6", "Small ledge on side of start pillar"},
        {"Rainbow Scale 7", "Inside waterfall cave"},
        {"Rainbow Scale 8", "Corner island beside waterfall cave"},
        {"Rainbow Scale 9", "Side of dead end path next to Juilius' lab"},
        {"Rainbow Scale 10", "Next to Extra Life"},
        {"Rainbow Scale 11", "Above water next to Julius' lab (Pontoon scale)"},
        {"Rainbow Scale 12", "After pillar platforms leading to Hub 3"},
        {"Rainbow Scale 13", "In the water next to pillar platforms leading up to Hub 3"},
        {"Rainbow Scale 14", "Behind rock at entrance to Hub 2"},
        {"Rainbow Scale 15", "Inside starting pillar (entrance on back side)"},
        {"Rainbow Scale 16", "Underwater near Julius' lab"},
        {"Rainbow Scale 17", "On wooden platform in Hub 3"},
        {"Rainbow Scale 18", "Under Thunder Egg collector in Hub 3"},
        {"Rainbow Scale 19", "Just past ice wall in Hub 2"},
        {"Rainbow Scale 20", "Next to water wheel on Julius' balcony"},
        {"Rainbow Scale 21", "Underwater near waterfall cave"},
        {"Rainbow Scale 22", "Just past Final Gauntlet gate"},
        {"Rainbow Scale 23", "Hidden around the corner in Final Gauntlet area"},
        {"Rainbow Scale 24", "Floating behind starting pillar"},
        {"Rainbow Scale 25", "Beneath thunder egg collector in Hub 1"}
    },
    {},
    {},
    {   
        {"Picture Frame 1", "Inside Julius' lab"},
        {"Picture Frame 2", "Above Talisman display platform"},
        {"Picture Frame 3", "Above Extra Life"},
        {"Picture Frame 4", "Corner island next to waterfall cave"},
        {"Picture Frame 5", "Above burning logs leading to Hub 3"},
        {"Picture Frame 6", "Near portal to Cass' Crest (Final Gauntlet area)"},
        {"Picture Frame 7", "Above center of path leading to Hub 1"},
        {"Picture Frame 8", "In Hub 2"},
        {"Picture Frame 9", "Between houses in Hub 1"}
    },
    { {"Signpost 1", "Starting platform"} },
    { {"Extra Life 1", "Dead end next to Julius' lab"} }
);

const LevelCollectibleInfo A1Info(
    "Two Up",
    {  },
    { 
        {"Golden Cog 1", "At Julius'"},
        {"Golden Cog 2", "Small platform in the water just past purple crab"},
        {"Golden Cog 3", "Wood platform near 20 opal ledge - In upper area"},
        {"Golden Cog 4", "Platform next to Second Rang"},
        {"Golden Cog 5", "Platform next to bridge before purple crab"},
        {"Golden Cog 6", "On pontoon in deep water - In upper area"},
        {"Golden Cog 7", "In cave near start"},
        {"Golden Cog 8", "Raised ledge before Julius"},
        {"Golden Cog 9", "Ledge near start - Next to Cog  7"},
        {"Golden Cog 10", "Tall platform next to 20 opal ledge - before cave leading to purple crab"} 
    },
    { 	
        {"Bilby Dad", "Next to Maurie Signpost"},
        {"Bilby Mum", "In corner before Julius"},
        {"Bilby Boy", "On ledge next to Julius"},
        {"Bilby Girl", "Behind platform next to bridge before purple crab"},
        {"Bilby Grandma", "Cave in upper area"}
    },
    {   
        {"Picture Frame 1", "At start"},
        {"Picture Frame 2", "Waterfall at top of steps by first bilby"},
        {"Picture Frame 3", "Between the two trees before cave leading to purple crab"},
        {"Picture Frame 4", "Near dunny on large steps leading down to the final water section"},
        {"Picture Frame 5", "Near the two crates in upper area"},
        {"Picture Frame 6", "Corner near slope down to second rang area (Near bunyip in upper area)"},
        {"Picture Frame 7", "Empty grass ledge near spy egg thunder egg"}
    },
    { 	
        {"Signpost 1", "At start (Jump)"},
        {"Signpost 2", "At first thunder egg (Thunder Eggs)"},
        {"Signpost 3", "Before purple crab (Lock-on)"},
        {"Signpost 4", "Near bunyip (Ty's View)"},
        {"Signpost 5", "At spy eggs (Bite Chain)"},
        {"Signpost 6", "Before Glide the Gap (Gliding)"},
        {"Signpost 7", "At pit with three crates near start (Bite)"},
        {"Signpost 8", "Before first frills (Rangs)"},
        {"Signpost 9", "Before first bridge (Camera)"},
        {"Signpost 10", "After first bridge (Opals)"},
        {"Signpost 11", "After Glide the Gap (Time Attack)"},
        {"Signpost 12", "At mushrooms next to opal collector (Mushrooms)"},
        {"Signpost 13", "At Second Rang (Rang the Frills)"},
        {"Signpost 14", "At first Bilby (Bilbies)"},
        {"Signpost 15", "At first Dunny (Dunnies)"}
    },
    {   
        {"Extra Life 1", "Waterfall cave near Rang the Frills"},
        {"Extra Life 2", "Underwater under barrier between purple crab bridge and upper area"}
    }
);

const LevelCollectibleInfo A2Info(
    "Walk in the Park",
    {  },
    {   
        {"Golden Cog 1", "Platform by frills near logs leading to slide 1"},
        {"Golden Cog 2", "Towards top of logs leading to slide 1 near two crates"},
        {"Golden Cog 3", "Behind waterfall at top of slide 1"},
        {"Golden Cog 4", "Slide 1 cave"},
        {"Golden Cog 5", "Wood platform in valley between slides"},
        {"Golden Cog 6", "Platform at bottom of slide 2"},
        {"Golden Cog 7", "Past first rhino beetle"},
        {"Golden Cog 8", "Platform halfway up boulder slopes"},
        {"Golden Cog 9", "Under bridge at end"},
        {"Golden Cog 10", "Behind starting platform"}
    },
    {   
        {"Bilby Dad", "At entrance to slide 1 cave"},
        {"Bilby Mum", "At far end of valley between slides"},
        {"Bilby Boy", "Logs area canyon straight from second log"},
        {"Bilby Girl", "Logs area canyon under final log"},
        {"Bilby Grandma", "Inside Bounce Tree"} 
    },
    {   
        {"Picture Frame 1", "Bottom of boulder slopes"},
        {"Picture Frame 2", "Above middle of valley between slides"},
        {"Picture Frame 3", "At start"},
        {"Picture Frame 4", "Inside lower entrance to bat cave"},
        {"Picture Frame 5", "In waterfall at end"},
        {"Picture Frame 6", "With bats in bat cave"}
    },
    {   
        {"Signpost 1", "On path near start (Shazza)"},
        {"Signpost 2", "Before first rhino beetle (Rang-proof enemies)"},
        {"Signpost 3", "Top of slide 1 (Waterslides)"}
    },
    { {"Extra Life 1", "In bat cave"} }
);

const LevelCollectibleInfo A3Info(
    "Ship Rex",
    {  },
    { 
        {"Golden Cog 1", "Floating platform to the right immediately after Gate 2"},
        {"Golden Cog 2", "Pillar to the right immediately after Gate 1"},
        {"Golden Cog 3", "Wood platform at start"},
        {"Golden Cog 4", "Large rockpool"},
        {"Golden Cog 5", "Floating platform reached from geyser"},
        {"Golden Cog 6", "Island in corner near end of coconuts (left of eels)"},
        {"Golden Cog 7", "Small rockpool near Bilby Dad"},
        {"Golden Cog 8", "Top of spire"},
        {"Golden Cog 9", "Floating platform near opal machine"},
        {"Golden Cog 10", "Large raised area in corner of ship wreck area"}
},
    { 
        {"Bilby Dad", "Wooden platform to the right immediately after Gate 1"},
        {"Bilby Mum", "Ledge beneath Nest Egg"},
        {"Bilby Boy", "Back side of spire shore"},
        {"Bilby Girl", "Island to the left immediately after eels"},
        {"Bilby Grandma", "On platform past end of coconuts"}
},
    { 
        {"Picture Frame 1", "Top of spire"},
        {"Picture Frame 2", "Small island in Ship Wreck area"},
        {"Picture Frame 3", "Ledge between Nest and Coconut start"},
        {"Picture Frame 4", "Side of island near tunnel into rock pool"},
        {"Picture Frame 5", "End of coconuts - Far"},
        {"Picture Frame 6", "Above shed at start"},
        {"Picture Frame 7", "Right side of porch at start"},
        {"Picture Frame 8", "Left side of porch at start"},
        {"Picture Frame 9", "End of coconuts - Near"} 
    },
    {  },
    { 
        {"Extra Life 1", "Small rockpool near tunnel entry to Ship Wreck area"},
        {"Extra Life 2", "On pillars in quicksand"},
        {"Extra Life 3", "On back platforms at top of spire"} 
    }
);

const LevelCollectibleInfo B1Info(
    "Bridge on the River Ty",
    {  },
    { 
        {"Golden Cog 1", "On pillar near big tree in starting area (glide down from higher area)"},
        {"Golden Cog 2", "Behind cobweb after first dunny"},
        {"Golden Cog 3", "Hidden in corner below first dunny"},
        {"Golden Cog 4", "On pillar in spider den"},
        {"Golden Cog 5", "Hidden to the side of steps near Dennis"},
        {"Golden Cog 6", "Dead tree near opal machine"},
        {"Golden Cog 7", "Platform protected by bats and pontoons in corner"},
        {"Golden Cog 8", "Platform in middle of water under large bridge"},
        {"Golden Cog 9", "Hollow log in Dennis' house"},
        {"Golden Cog 10", "Under small bridge along Dennis escort path"}
    },
    { 
        {"Bilby Dad", "Behind lower cobweb before broken bridge"},
        {"Bilby Mum", "Behind upper cobweb before broken bridge"},
        {"Bilby Boy", "Cave in starting area"},
        {"Bilby Girl", "Beside Neddy's Den"},
        {"Bilby Grandma", "Behind tree between Dennis starting location and Rex Diving"}
    },
    {
        {"Picture Frame 1", "Above Neddy bilby (Above bridge)"},
        {"Picture Frame 2", "Ledge near large tree - Large tree (Glide to cog)"},
        {"Picture Frame 3", "Corner next to bilby mum"},
        {"Picture Frame 4", "Ledge over first bridge"},
        {"Picture Frame 5", "Under big tree next to PF 2"},
        {"Picture Frame 6", "Next to Rex Diving"},
        {"Picture Frame 7", "Under big tree at start - Before 1st Bridge"},
        {"Picture Frame 8", "Next to Neddy bilby (Between burner and bridge)"},
        {"Picture Frame 9", "Above gap in big bridge (near Neddy)"},
        {"Picture Frame 10", "Under tree at Ramp onto big bridge"},
        {"Picture Frame 11", "Under waterfall before spider den"},
        {"Picture Frame 12", "Dennis House"},
        {"Picture Frame 13", "Above big bridge - Dennis House side"},
        {"Picture Frame 14", "Under big bridge - Dennis House side"},
        {"Picture Frame 15", "Corner near moving platforms"},
        {"Picture Frame 16", "Above pontoon next to Dennis Starting location"},
        {"Picture Frame 17", "Neddy Den 1"},
        {"Picture Frame 18", "Neddy Den 2"},
        {"Picture Frame 19", "Neddy Den 3"},
        {"Picture Frame 20", "Neddy Den 4"} 
    },
    { 
        {"Signpost 1", "Near Neddy (Neddy)"},
        {"Signpost 2", "At start (Dennis)"}
    },
    {  }
);

const LevelCollectibleInfo B2Info(
    "Snow Worries",
    {  },
    { 
        {"Golden Cog 1", "Pillar platforming on the right side of right ice path at start"},
        {"Golden Cog 2", "Middle of hole in ice above ice sheet"},
        {"Golden Cog 3", "Pillar at the base of mountain steps"},
        {"Golden Cog 4", "Underwater before gate"},
        {"Golden Cog 5", "Behind house at start"},
        {"Golden Cog 6", "On central raised area in ice valley"},
        {"Golden Cog 7", "Underwater behind gate"},
        {"Golden Cog 8", "Hidden cave beyond icicle cave"},
        {"Golden Cog 9", "Small platform on right side ice path before bilby and opal ring around tree"},
        {"Golden Cog 10", "On beam above arena steps"}
    },
    {
        {"Bilby Dad", "After pillars with two koala kids - before ice path starts up again"},
        {"Bilby Mum", "Behind Ice Blocks"},
        {"Bilby Boy", "Behind mill ramp leading up to Icicle Cave"},
        {"Bilby Girl", "Pillar at end of floating ice platforms near Icicle Cave"},
        {"Bilby Grandma", "Behind tree near ice holes"} 
    },
    { 
        {"Picture Frame 1", "Above round dirt patch at start"},
        {"Picture Frame 2", "Above round dirt patch by left ice hole"},
        {"Picture Frame 3", "Next to boonie (Above snow)"},
        {"Picture Frame 4", "Opposite cog 9"},
        {"Picture Frame 5", "Ice Spike area"},
        {"Picture Frame 6", "Start of pillar platforming - Right side at start"},
        {"Picture Frame 7", "Behind pillar platforming - Right side at start"},
        {"Picture Frame 8", "Corner before Right Ice Hole"},
        {"Picture Frame 9", "Corner after Right Ice Hole (Near frills)"},
        {"Picture Frame 10", "Above round dirt patch near bilby dad"},
        {"Picture Frame 11", "Next to boonie (Above ice)"},
        {"Picture Frame 12", "Near ice blocks - higher on slope"},
        {"Picture Frame 13", "Behind ice blocks"},
        {"Picture Frame 14", "Above round dirt patch near icicle cave"},
        {"Picture Frame 15", "Left side - 1st indent"},
        {"Picture Frame 16", "Left side - 3rd indent"},
        {"Picture Frame 17", "Left side - 2nd indent"},
        {"Picture Frame 18", "Corner after left ice hole"},
        {"Picture Frame 19", "Behind house"},
        {"Picture Frame 20", "Near Mim"},
        {"Picture Frame 21", "Bottom of center path - on left before left ice hole (above snow)"},
        {"Picture Frame 22", "Left of path leading to mountain steps"},
        {"Picture Frame 23", "Bottom of right side path (On snow past tree bilby)"},
        {"Picture Frame 24", "Between PF 8 and PF 23"} 
    },
    { 
        {"Signpost 1", "Start on opal path (Ice)"},
        {"Signpost 2", "Bottom of main path pillars start of ice (Thin Ice)"},
        {"Signpost 3", "Bottom of mill mountain (Mill)"}
    },
    {  }
);

const LevelCollectibleInfo B3Info(
    "Outback Safari",
    {  },
    { 
        {"Golden Cog 1", "On ledge after gap jump in upper waterfall area"},
        {"Golden Cog 2", "Behind rock in lower waterfall cave"},
        {"Golden Cog 3", "In ring of hay bales towards end of Shazza escort path"},
        {"Golden Cog 4", "Inside shed between emus and Shazza escort path"},
        {"Golden Cog 5", "Inside shed down right path near start"},
        {"Golden Cog 6", "Inside shed in emus area"},
        {"Golden Cog 7", "Inside shed in water towers area"},
        {"Golden Cog 8", "Inside shed in corner of lower waterfall area"},
        {"Golden Cog 9", "In ring of hay bales near path leading up to upper area"},
        {"Golden Cog 10", "In shed on spiral path leading to upper area"}
    },
    { 
        {"Bilby Dad", "In metal shed next to opal collector"},
        {"Bilby Mum", "In metal shed in Emu Roundup area"},
        {"Bilby Boy", "Near the final water tower"},
        {"Bilby Girl", "In metal shed in centre of ring of crates on Shazza's escort path"},
        {"Bilby Grandma", "Under waterfal next to Toxic Trouble"}
    },
    {  },
    {  },
    { 
        {"Extra Life 1", "In metal shed in water towers area"},
        {"Extra Life 2", "On spiral path up the mountain"},
        {"Extra Life 3", "In metal shed on left path from start"}
    }
);

const LevelCollectibleInfo D4Info(
    "Crikey's Cove",
    {  },
    {  },
    {  },
    {  },
    {  },
    { {"Extra Life 1", "Top of lighthouse through underwater tunnel"} }
);

const LevelCollectibleInfo C1Info(
    "Lyre, Lyre Pants on Fire",
    {  },
    { 
        {"Golden Cog 1", "Hidden by trees on right at bottom of slide"},
        {"Golden Cog 2", "On pillar at the bottom of slide"},
        {"Golden Cog 3", "On pillar near spy eggs and bilby in alcove"},
        {"Golden Cog 4", "Ice block"},
        {"Golden Cog 5", "On metal pillar next to water worries log entrance"},
        {"Golden Cog 6", "Bounce tree"},
        {"Golden Cog 7", "On pillar before slide"},
        {"Golden Cog 8", "Spy egg pillar after first log teleport"},
        {"Golden Cog 9", "Hidden behind cobweb in lever area"},
        {"Golden Cog 10", "Pillar between 'jump to the other side' broken bridge"} 
    },
    { 
        {"Bilby Dad", "At end under gap in path to end potal"},
        {"Bilby Mum", "In alcove near Muddy Towers area"},
        {"Bilby Boy", "Next to opal collector on higher path"},
        {"Bilby Girl", "Behind bounce tree root"},
        {"Bilby Grandma", "Under waterfall at top of slide"} 
    },
    { 
        {"Picture Frame 1", "Between 'Jump to the other side' gap"},
        {"Picture Frame 2", "At Start"},
        {"Picture Frame 3", "Bottom of slide"},
        {"Picture Frame 4", "Next to bounce tree"},
        {"Picture Frame 5", "Next to dunny @ top of slide"} 
    },
    {  },
    {  }
);

const LevelCollectibleInfo C2Info(
    "Beyond the Black Stump",
    {  },
    { 
        {"Golden Cog 1", "Hidden alcove after pillar ponder thunder egg"},
        {"Golden Cog 2", "Snow pile at bottom of cliff area at end"},
        {"Golden Cog 3", "On platform in logs area"},
        {"Golden Cog 4", "On ledge on cliff at end"},
        {"Golden Cog 5", "At end of opal path on ground at bottom of cliff area at end"},
        {"Golden Cog 6", "Ice cave spire"},
        {"Golden Cog 7", "Ice block"},
        {"Golden Cog 8", "Pillar near to Elizabeth in corner"},
        {"Golden Cog 9", "Warp flower cog"},
        {"Golden Cog 10", "Next to portal at top of cable cars"}
    },
    { 
        {"Bilby Dad", "Behind cobweb near bottom of cable cars"},
        {"Bilby Mum", "Behind cobweb in log area"},
        {"Bilby Boy", "Inside ice cave on bottom level"},
        {"Bilby Girl", "Warpflowers on bottom level"},
        {"Bilby Grandma", "Hidden alcove near four pillars"} 
    },
    { 
        {"Picture Frame 1", "Behind cable car platform house"},
        {"Picture Frame 2", "Corner near cable car platform house near ice"},
        {"Picture Frame 3", "Corner past cable car platform house 1"},
        {"Picture Frame 4", "Corner past cable car platform house 2"},
        {"Picture Frame 5", "Ski slope level 4 - Right Side"},
        {"Picture Frame 6", "Ski slope level 4 - left side"},
        {"Picture Frame 7", "Ski slope level 2"},
        {"Picture Frame 8", "Ski slope level 3"},
        {"Picture Frame 9", "Ski slope level 1"},
        {"Picture Frame 10", "Above fire logs near Rafflesia to Liz"},
        {"Picture Frame 11", "Behind fire at start"},
        {"Picture Frame 12", "Above fire opposite Katie"},
        {"Picture Frame 13", "Between Boonie and Liz"},
        {"Picture Frame 14", "Center of valley leading to Katie"},
        {"Picture Frame 15", "Past 3rd flower in flower chain"},
        {"Picture Frame 16", "Near ice block cog"},
        {"Picture Frame 17", "Above crossroads at start"},
        {"Picture Frame 18", "Under final log in log climb"},
        {"Picture Frame 19", "Next to bunyip in pillar area"},
        {"Picture Frame 20", "Straight ahead after log climb"},
        {"Picture Frame 21", "Left side after first big ice patch after log climb"},
        {"Picture Frame 22", "Between cable car platform and bilby mum"},
        {"Picture Frame 23", "Just before Bad Boonie"},
        {"Picture Frame 24", "Above platform start of final log in log climb"},
        {"Picture Frame 25", "Above blue tongues"},
        {"Picture Frame 26", "End area next to cliff ledge cog"},
        {"Picture Frame 27", "Next to Cog 5"},
        {"Picture Frame 28", "In trees - Turret area 1"},
        {"Picture Frame 29", "In trees - Turret area 2"} 
    },
    { {"Signpost 1", "Top of log climb (Catch Boonie)"} },
    {  }
);

const LevelCollectibleInfo C3Info(
    "Rex Marks the Spot",
    {  },
    { 
        {"Golden Cog 1", "Next to portal at top of cable cars"},
        {"Golden Cog 2", "Underwater near volcano thunder egg and warp flower cog"},
        {"Golden Cog 3", "On moving platform"},
        {"Golden Cog 4", "On large platform near skull rock at start"},
        {"Golden Cog 5", "Underwater near eels between coconut shores and bald island !!!!!FRANK!!!!!!"},
        {"Golden Cog 6", "Bald island cog"},
        {"Golden Cog 7", "Volcano cog"},
        {"Golden Cog 8", "Coconut shores cog (on floating platform requiring button hit)"},
        {"Golden Cog 9", "Anchor island"},
        {"Golden Cog 10", "Warp flower cog"} 
    },
    { 
        {"Bilby Dad", "Crab island"},
        {"Bilby Mum", "Between two rocks"},
        {"Bilby Boy", "Warpflower bilby"},
        {"Bilby Girl", "Coconut shores"},
        {"Bilby Grandma", "Pillar bilby"}
    },
    { 
        {"Picture Frame 1", "Bald island"},
        {"Picture Frame 2", "Anchor island"},
        {"Picture Frame 3", "Coconut shores near geyser"},
        {"Picture Frame 4", "Coconut shores near cog in water towards bald island"},
        {"Picture Frame 5", "Coconut shores near shores chest"},
        {"Picture Frame 6", "Figure 8 island towards coconut shores floating platform cog"},
        {"Picture Frame 7", "Figure 8 island opposite PF 6 towards PF 5"},
        {"Picture Frame 8", "Figure 8 island opposite PF 6 away from PF 5 next to PF 7"},
        {"Picture Frame 9", "Skull rock"},
        {"Picture Frame 10", "Figure 8 island on the right of opal path at start"},
        {"Picture Frame 11", "Crab island towards cog underwater near warp flower cog"},
        {"Picture Frame 12", "Crab island closest to fence near bilby"},
        {"Picture Frame 13", "Crab island on approach from opal path"},
        {"Picture Frame 14", "Island on the right of opal path leading to crab island next to dunny"},
        {"Picture Frame 15", "Same island as PF 14 near sunken ship"},
        {"Picture Frame 16", "Same island as PF 15 opposite side"},
        {"Picture Frame 17", "Outside volcano entrance"},
        {"Picture Frame 18", "Inside volcano"}
    },
    {  },
    { 
        {"Extra Life 1", "Inside volcano"},
        {"Extra Life 2", "Underwater near big red button for platform cog"}
    }
);

const LevelCollectibleInfo E1Info(
    "Cass' Pass",
    {  },
    {  },
    {  },
    {  },
    { {"Signpost 1", "At end"} },
    { 
        {"Extra Life 1", "Before island with crest portal on left side underwater"},
        {"Extra Life 2", "Behind island with Crest portal at end underwater"},
        {"Extra Life 3", "Behind tree root at first waterfall"},
        {"Extra Life 4", "In corner under lower waterfall after slide"}
    }
);

const LevelCollectibleInfo D2Info(
    "Cass' Crest",
    {  },
    {  },
    {  },
    {  },
    {  },
    { 
        {"Extra Life 1", "On platforms before second button"},
        {"Extra Life 2", "On platform to the left of path next to second dunny"},
        {"Extra Life 3", "At the end around a corner"},
        {"Extra Life 4", "Underwater at start under the path between where Shazza is and where Shadow is"},
        {"Extra Life 5", "In hole past third gas jet"}
    }
);