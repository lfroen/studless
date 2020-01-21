#ifndef LDRAW_COLOR_DEFS

struct { uint32_t color, complement; const char *name; } g_ldrawColors[] = {
	{.color = 0xff1d1305, .complement = 0xff595959, .name = "Black"}, // Black
	{.color = 0xffbf5500, .complement = 0xff333333, .name = "Blue"}, // Blue
	{.color = 0xff3e7a25, .complement = 0xff333333, .name = "Green"}, // Green
	{.color = 0xff8f8300, .complement = 0xff333333, .name = "Dark Turquoise"}, // Dark_Turquoise
	{.color = 0xff091ac9, .complement = 0xff333333, .name = "Red"}, // Red
	{.color = 0xffa070c8, .complement = 0xff333333, .name = "Dark Pink"}, // Dark_Pink
	{.color = 0xff273958, .complement = 0xff1e1e1e, .name = "Brown"}, // Brown
	{.color = 0xff9da19b, .complement = 0xff333333, .name = "Light Gray"}, // Light_Gray
	{.color = 0xff5c6e6d, .complement = 0xff333333, .name = "Dark Gray"}, // Dark_Gray
	{.color = 0xffe3d2b4, .complement = 0xff333333, .name = "Light Blue"}, // Light_Blue
	{.color = 0xff4a9f4b, .complement = 0xff333333, .name = "Bright Green"}, // Bright_Green
	{.color = 0xffafa555, .complement = 0xff333333, .name = "Light Turquoise"}, // Light_Turquoise
	{.color = 0xff5e70f2, .complement = 0xff333333, .name = "Salmon"}, // Salmon
	{.color = 0xffac97fc, .complement = 0xff333333, .name = "Pink"}, // Pink
	{.color = 0xff37cdf2, .complement = 0xff333333, .name = "Yellow"}, // Yellow
	{.color = 0xffffffff, .complement = 0xff333333, .name = "White"}, // White
	{.color = 0xff7f7f7f, .complement = 0xff333333, .name = "Main Colour"}, // Main_Colour
	{.color = 0xffb8dac2, .complement = 0xff333333, .name = "Light Green"}, // Light_Green
	{.color = 0xff96e6fb, .complement = 0xff333333, .name = "Light Yellow"}, // Light_Yellow
	{.color = 0xff9ecde4, .complement = 0xff333333, .name = "Tan"}, // Tan
	{.color = 0xffe2cac9, .complement = 0xff333333, .name = "Light Violet"}, // Light_Violet
	{.color = 0xfab0ffe0, .complement = 0xff74c3a4, .name = "Glow In Dark Opaque"}, // Glow_In_Dark_Opaque
	{.color = 0xff7b0081, .complement = 0xff333333, .name = "Purple"}, // Purple
	{.color = 0xffb03220, .complement = 0xff1e1e1e, .name = "Dark Blue Violet"}, // Dark_Blue_Violet
	{.color = 0xff7f7f7f, .complement = 0xff333333, .name = "Edge Colour"}, // Edge_Colour
	{.color = 0xff188afe, .complement = 0xff333333, .name = "Orange"}, // Orange
	{.color = 0xff783992, .complement = 0xff333333, .name = "Magenta"}, // Magenta
	{.color = 0xff0be9bb, .complement = 0xff333333, .name = "Lime"}, // Lime
	{.color = 0xff738a95, .complement = 0xff333333, .name = "Dark Tan"}, // Dark_Tan
	{.color = 0xffc8ade4, .complement = 0xff333333, .name = "Bright Pink"}, // Bright_Pink
	{.color = 0xffba78ac, .complement = 0xff333333, .name = "Medium Lavender"}, // Medium_Lavender
	{.color = 0xffedd5e1, .complement = 0xff333333, .name = "Lavender"}, // Lavender
	{.color = 0xc8000000, .complement = 0xff333333, .name = "Trans Black IR Lens"}, // Trans_Black_IR_Lens
	{.color = 0x80a02000, .complement = 0xff640000, .name = "Trans Dark Blue"}, // Trans_Dark_Blue
	{.color = 0x80417823, .complement = 0xff39621e, .name = "Trans Green"}, // Trans_Green
	{.color = 0x8046e656, .complement = 0xff6ba89d, .name = "Trans Bright Green"}, // Trans_Bright_Green
	{.color = 0x80091ac9, .complement = 0xff000088, .name = "Trans Red"}, // Trans_Red
	{.color = 0x809566df, .complement = 0xff592aa3, .name = "Trans Dark Pink"}, // Trans_Dark_Pink
	{.color = 0x800d80ff, .complement = 0xff0024bd, .name = "Trans Neon Orange"}, // Trans_Neon_Orange
	{.color = 0x80f0dfc1, .complement = 0xffb4a385, .name = "Trans Very Light Blue"}, // Trans_Very_Light_Blue
	{.color = 0x80525f63, .complement = 0xff161317, .name = "Trans Black"}, // Trans_Black
	{.color = 0x80b79a55, .complement = 0xff736919, .name = "Trans Medium Blue"}, // Trans_Medium_Blue
	{.color = 0x8000ffc0, .complement = 0xff00c384, .name = "Trans Neon Green"}, // Trans_Neon_Green
	{.color = 0x80efe9ae, .complement = 0xffb0b372, .name = "Trans Light Blue"}, // Trans_Light_Blue
	{.color = 0x809f7096, .complement = 0xff63345a, .name = "Trans Light Purple"}, // Trans_Light_Purple
	{.color = 0x80ac97fc, .complement = 0xff8c71a8, .name = "Trans Pink"}, // Trans_Pink
	{.color = 0x802fcdf5, .complement = 0xff00748e, .name = "Trans Yellow"}, // Trans_Yellow
	{.color = 0x80fcfcfc, .complement = 0xffc3c3c3, .name = "Trans Clear"}, // Trans_Clear
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x80cba5a5, .complement = 0xff250028, .name = "Trans Purple"}, // Trans_Purple
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x8000b0da, .complement = 0xff3fbac3, .name = "Trans Neon Yellow"}, // Trans_Neon_Yellow
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x801c8ff0, .complement = 0xff285ca4, .name = "Trans Orange"}, // Trans_Orange
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0xff4c5a64, .complement = 0xff101e28, .name = "Chrome Antique Brass"}, // Chrome_Antique_Brass
	{.color = 0xffbf966c, .complement = 0xff682a20, .name = "Chrome Blue"}, // Chrome_Blue
	{.color = 0xff71b33c, .complement = 0xff357700, .name = "Chrome Green"}, // Chrome_Green
	{.color = 0xff8e4daa, .complement = 0xff52116e, .name = "Chrome Pink"}, // Chrome_Pink
	{.color = 0xff342a1b, .complement = 0xff595959, .name = "Chrome Black"}, // Chrome_Black
	{.color = 0xff2fcdf5, .complement = 0xff333333, .name = "Rubber Yellow"}, // Rubber_Yellow
	{.color = 0x8000b0ca, .complement = 0xff00748e, .name = "Rubber Trans Yellow"}, // Rubber_Trans_Yellow
	{.color = 0x80ffffff, .complement = 0xffc3c3c3, .name = "Rubber Trans Clear"}, // Rubber_Trans_Clear
	{.color = 0xff9bcff3, .complement = 0xff333333, .name = "Very Light Orange"}, // Very_Light_Orange
	{.color = 0xff9862cd, .complement = 0xff333333, .name = "Light Purple"}, // Light_Purple
	{.color = 0xff122a58, .complement = 0xff595959, .name = "Reddish Brown"}, // Reddish_Brown
	{.color = 0xffa9a5a0, .complement = 0xff333333, .name = "Light Bluish Gray"}, // Light_Bluish_Gray
	{.color = 0xff686e6c, .complement = 0xff333333, .name = "Dark Bluish Gray"}, // Dark_Bluish_Gray
	{.color = 0xffd19d5c, .complement = 0xff333333, .name = "Medium Blue"}, // Medium_Blue
	{.color = 0xffa1dc73, .complement = 0xff333333, .name = "Medium Green"}, // Medium_Green
	{.color = 0xff000000, .complement = 0xff3860ab, .name = "Speckle Black Copper"}, // Speckle_Black_Copper
	{.color = 0xff615f63, .complement = 0xff888789, .name = "Speckle Dark Bluish Gray Silver"}, // Speckle_Dark_Bluish_Gray_Silver
	{.color = 0xffcfccfe, .complement = 0xff333333, .name = "Light Pink"}, // Light_Pink
	{.color = 0xffb3d7f6, .complement = 0xff333333, .name = "Light Flesh"}, // Light_Flesh
	{.color = 0xe0ffffff, .complement = 0xffc3c3c3, .name = "Milky White"}, // Milky_White
	{.color = 0xffb4a9a5, .complement = 0xff333333, .name = "Metallic Silver"}, // Metallic_Silver
	{.color = 0xff5f9b89, .complement = 0xff333333, .name = "Metallic Green"}, // Metallic_Green
	{.color = 0xff34acdb, .complement = 0xff333333, .name = "Metallic Gold"}, // Metallic_Gold
	{.color = 0xff31281a, .complement = 0xff333333, .name = "Metallic Black"}, // Metallic_Black
	{.color = 0xff2a70cc, .complement = 0xff333333, .name = "Medium Dark Flesh"}, // Medium_Dark_Flesh
	{.color = 0xff91363f, .complement = 0xff1e1e1e, .name = "Dark Purple"}, // Dark_Purple
	{.color = 0xff3a507c, .complement = 0xff333333, .name = "Dark Flesh"}, // Dark_Flesh
	{.color = 0xff5c6e6d, .complement = 0xff333333, .name = "Metallic Dark Gray"}, // Metallic_Dark_Gray
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0xffdb614c, .complement = 0xff333333, .name = "Blue Violet"}, // Blue_Violet
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0xff6891d0, .complement = 0xff333333, .name = "Flesh"}, // Flesh
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0xffbdbafe, .complement = 0xff333333, .name = "Light Salmon"}, // Light_Salmon
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0xffa35443, .complement = 0xff333333, .name = "Violet"}, // Violet
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0xffca7468, .complement = 0xff333333, .name = "Medium Violet"}, // Medium_Violet
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x809566df, .complement = 0xff662a9a, .name = "Glitter Trans Dark Pink"}, // Glitter_Trans_Dark_Pink
	{.color = 0xff3cd2c7, .complement = 0xff333333, .name = "Medium Lime"}, // Medium_Lime
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x80ffffff, .complement = 0xffc3c3c3, .name = "Glitter Trans Clear"}, // Glitter_Trans_Clear
	{.color = 0xffd1d7b3, .complement = 0xff333333, .name = "Aqua"}, // Aqua
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0xffa7e4d9, .complement = 0xff333333, .name = "Light Lime"}, // Light_Lime
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0xff61baf9, .complement = 0xff333333, .name = "Light Orange"}, // Light_Orange
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x80610064, .complement = 0xff250028, .name = "Glitter Trans Purple"}, // Glitter_Trans_Purple
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0xff000000, .complement = 0xff888789, .name = "Speckle Black Silver"}, // Speckle_Black_Silver
	{.color = 0xff000000, .complement = 0xff34acdb, .name = "Speckle Black Gold"}, // Speckle_Black_Gold
	{.color = 0xff274a96, .complement = 0xff333333, .name = "Copper"}, // Copper
	{.color = 0xffa8a39c, .complement = 0xff333333, .name = "Pearl Light Gray"}, // Pearl_Light_Gray
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0xffba7756, .complement = 0xff333333, .name = "Metal Blue"}, // Metal_Blue
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0xff61bedc, .complement = 0xff333333, .name = "Pearl Light Gold"}, // Pearl_Light_Gold
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0xff575857, .complement = 0xff333333, .name = "Pearl Dark Gray"}, // Pearl_Dark_Gray
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0xffbcbdbb, .complement = 0xff333333, .name = "Pearl Very Light Gray"}, // Pearl_Very_Light_Gray
	{.color = 0xffe0e3e6, .complement = 0xff333333, .name = "Very Light Bluish Gray"}, // Very_Light_Bluish_Gray
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0xff3e88b4, .complement = 0xff333333, .name = "Flat Dark Gold"}, // Flat_Dark_Gold
	{.color = 0xff888789, .complement = 0xff333333, .name = "Flat Silver"}, // Flat_Silver
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0xfff2f3f2, .complement = 0xff333333, .name = "Pearl White"}, // Pearl_White
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0xff3dbbf8, .complement = 0xff333333, .name = "Bright Light Orange"}, // Bright_Light_Orange
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0xffe1c186, .complement = 0xff333333, .name = "Bright Light Blue"}, // Bright_Light_Blue
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0xff0410b3, .complement = 0xff333333, .name = "Rust"}, // Rust
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0xff3af0ff, .complement = 0xff333333, .name = "Bright Light Yellow"}, // Bright_Light_Yellow
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0xffd6be56, .complement = 0xff333333, .name = "Sky Blue"}, // Sky_Blue
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0xff212121, .complement = 0xff595959, .name = "Rubber Black"}, // Rubber_Black
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0xff5b320d, .complement = 0xff1e1e1e, .name = "Dark Blue"}, // Dark_Blue
	{.color = 0xffb23300, .complement = 0xff333333, .name = "Rubber Blue"}, // Rubber_Blue
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0xff324618, .complement = 0xff595959, .name = "Dark Green"}, // Dark_Green
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0xfaadc6bd, .complement = 0xff718a81, .name = "Glow In Dark Trans"}, // Glow_In_Dark_Trans
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0xff2b9ccc, .complement = 0xff333333, .name = "Pearl Gold"}, // Pearl_Gold
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0xff002135, .complement = 0xff595959, .name = "Dark Brown"}, // Dark_Brown
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0xffc8a954, .complement = 0xff333333, .name = "Maersk Blue"}, // Maersk_Blue
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0xff0f0e72, .complement = 0xff333333, .name = "Dark Red"}, // Dark_Red
	{.color = 0xffd79814, .complement = 0xff333333, .name = "Dark Azure"}, // Dark_Azure
	{.color = 0xffddc23e, .complement = 0xff333333, .name = "Medium Azure"}, // Medium_Azure
	{.color = 0xffd8dcbd, .complement = 0xff333333, .name = "Light Aqua"}, // Light_Aqua
	{.color = 0xff2600c4, .complement = 0xff333333, .name = "Rubber Red"}, // Rubber_Red
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0xffa5eedf, .complement = 0xff333333, .name = "Yellowish Green"}, // Yellowish_Green
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0xff5a9a9b, .complement = 0xff333333, .name = "Olive Green"}, // Olive_Green
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0xff3da5bb, .complement = 0xff3db2bb, .name = "Chrome Gold"}, // Chrome_Gold
	{.color = 0xff7275d6, .complement = 0xff333333, .name = "Sand Red"}, // Sand_Red
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0xff1066d0, .complement = 0xff333333, .name = "Rubber Orange"}, // Rubber_Orange
	{.color = 0xffb185f7, .complement = 0xff333333, .name = "Medium Dark Pink"}, // Medium_Dark_Pink
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0xff1c9cfa, .complement = 0xff333333, .name = "Earth Orange"}, // Earth_Orange
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0xff845e84, .complement = 0xff333333, .name = "Sand Purple"}, // Sand_Purple
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0xffc1c2c1, .complement = 0xff333333, .name = "Rubber Light Gray"}, // Rubber_Light_Gray
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0xffacbca0, .complement = 0xff333333, .name = "Sand Green"}, // Sand_Green
	{.color = 0xff847159, .complement = 0xff333333, .name = "Sand Blue"}, // Sand_Blue
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0xffe0e0e0, .complement = 0xffa4a4a4, .name = "Chrome Silver"}, // Chrome_Silver
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0xff681d00, .complement = 0xff595959, .name = "Rubber Dark Blue"}, // Rubber_Dark_Blue
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0xff7b0081, .complement = 0xff333333, .name = "Rubber Purple"}, // Rubber_Purple
	{.color = 0xff507bb6, .complement = 0xff333333, .name = "Fabuland Brown"}, // Fabuland_Brown
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0xff0ba7ff, .complement = 0xff333333, .name = "Medium Orange"}, // Medium_Orange
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0xff0055a9, .complement = 0xff333333, .name = "Dark Orange"}, // Dark_Orange
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0xff00f0d7, .complement = 0xff333333, .name = "Rubber Lime"}, // Rubber_Lime
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0xff616765, .complement = 0xff595959, .name = "Magnet"}, // Magnet
	{.color = 0xffd0d0d0, .complement = 0xff333333, .name = "Electric Contact Alloy"}, // Electric_Contact_Alloy
	{.color = 0xff597aae, .complement = 0xff333333, .name = "Electric Contact Copper"}, // Electric_Contact_Copper
	{.color = 0xffa4a2a3, .complement = 0xff333333, .name = "Rubber Light Bluish Gray"}, // Rubber_Light_Bluish_Gray
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0xffdae3e6, .complement = 0xff333333, .name = "Very Light Gray"}, // Very_Light_Gray
	{.color = 0xff888789, .complement = 0xff333333, .name = "Rubber Flat Silver"}, // Rubber_Flat_Silver
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0x00000000, .complement = 0x00000000, .name = ""}, // 
	{.color = 0xfffafafa, .complement = 0xff333333, .name = "Rubber White"}, // Rubber_White
};

#endif // LDRAW_COLOR_DEFS
