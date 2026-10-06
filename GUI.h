#include <queue>

#define 黑色 FLinearColor(0, 0, 0, 1.f)
#define 白色 FLinearColor(1.f, 1.f, 1.f, 1.f)
#define 红色 FLinearColor(1.f, 0, 0, 1.f)
#define 透红 FLinearColor(0.5f, 0, 0, 0.5f)
#define 透绿 FLinearColor(0, 0.5, 0, 0.5)
#define 绿色 FLinearColor(0, 1.f, 0, 0)
#define 空白 FLinearColor(0, 0, 0, 0)
#define 蓝色 FLinearColor(0.24f, 0.52f, 0.78f, 1.f)
#define 灰色 FLinearColor(0.75f, 0.75f, 0.75f, 0.75f )
#define 灰灰色 FLinearColor(0.07f, 0.07, 0.07f, 1.f )
#define COLOR_BLACK FLinearColor(0, 0, 0, 1.f)
#define COLOR_WHITE FLinearColor(1.f, 1.f, 1.f, 1.f)
#define COLOR_RED FLinearColor(1.f, 0, 0, 1.f)
#define COLOR_CAR FLinearColor(1.f, 0.5f, 1.f, 1.f)
#define COLOR_GREEN FLinearColor(0, 0.5f, 0, 1.f)
#define COLOR_ORANGE FLinearColor(1.f, 0.4f, 0, 1.f)
#define COLOR_YELLOW FLinearColor(1.f, 1.f, 0, 1.f)
#define COLOR_LEAD FLinearColor(0.14f, 0.14f, 0.14f, 1.0f)
#define COLOR_LIME FLinearColor(0, 1.f, 0, 1.f)
#define COLOR_BLUE FLinearColor(0, 0, 1.f, 1.f)
#define COLOR_THISTLE FLinearColor(1.0f, 0.74f, 0.84f, 1.0f)
#define COLOR_PINK FLinearColor(1.0f, 0.75f, 0.8f, 1.0f)
#define COLOR_PLAYER FLinearColor(1.000f, 0.620f, 0.150f, 1.000f)

#define COLOR_BLACK_ALPHA(a) FLinearColor(0.0f, 0.0f, 0.0f, ((float)(a) / 255.0f))
#define COLOR_RED_ALPHA FLinearColor(0.f, 0, 0, 0.80f)

#define COLOR_BLACK2 FLinearColor(0, 0, 0, 0.7f)

FLinearColor 浅蓝 = FLinearColor(36 / 255.f, 249 / 255.f, 217 / 255.f, 255 / 255.f);

#define TSL_FONT_DEFAULT_SIZE 12

static UFont *tslFont = 0, *robotoFont = 0;
typedef unsigned char BYTE;
typedef uint32_t UINT32;
static void* tslFontUI = NULL;
UTexture2D* BG;
bool KeyCtrl;
bool KeyShift;
bool KeyAlt;
bool KeySuper;
static std::map<int32_t, std::queue<int32_t>> g_KeyEventQueues;
bool IsKeyDown(int user_key_index);
static bool 窗口 = true;

void DrawLine(UCanvas* Canvas, FVector2D posFrom, FVector2D posTo, float Thickness, FLinearColor Color) {
    Canvas->K2_DrawLine({posFrom.X, posFrom.Y}, {posTo.X, posTo.Y}, Thickness, Color);
}

void DrawBox(UCanvas* Canvas, FVector2D Pos, FVector2D size, FLinearColor Color) {
	Canvas->K2_DrawTexture(BG, Pos, size, {}, {}, Color, EBlendMode::BLEND_Translucent, 0, {});
}

void DrawText(UCanvas* Canvas, FString Text, FVector2D Pos, FLinearColor Color, FLinearColor OutlineColor, int FontSize, bool isCenter) {
	tslFont->LegacyFontSize = FontSize;
	Canvas->K2_DrawText(tslFont, Text, Pos, Color, 1.f, {}, {}, isCenter, false, true, OutlineColor);
	tslFont->LegacyFontSize = 13;
}

void DrawFilledRect(UCanvas* Canvas, FVector2D initial_pos, float w, float h, FLinearColor Color)
{
DrawBox(Canvas,initial_pos, {w,h}, Color);
}

void DrawFilledCircle2(UCanvas* Canvas, FVector2D Center, float Radius, FLinearColor Color) {
    int NumSides = 32;
    Canvas->K2_DrawPolygon(BG, Center, FVector2D(Radius, Radius), NumSides, Color);
}

void DrawRoundRect(UCanvas* Canvas, FVector2D pos, float w, float h, float r, FLinearColor color)
{
    
    DrawFilledCircle2(Canvas, FVector2D{ pos.X + r , pos.Y + r }, r, color);
    DrawFilledCircle2(Canvas, FVector2D{ pos.X + w - r , pos.Y + r }, r, color);
    DrawFilledCircle2(Canvas, FVector2D{ pos.X + w - r , pos.Y + h - r }, r, color);
    DrawFilledCircle2(Canvas, FVector2D{ pos.X + r, pos.Y + h - r}, r, color);
    
    DrawFilledRect(Canvas,FVector2D{ pos.X, pos.Y + r }, w , h - r * 2  , color);
    DrawFilledRect(Canvas,FVector2D{ pos.X+r, pos.Y }, w - r * 2 , r , color);
    DrawFilledRect(Canvas,FVector2D{ pos.X+r, pos.Y+h-r }, w - r * 2 , r , color);
}

void DrawRectangle(UCanvas* Canvas, FVector2D Pos, float Width, float Height, float Thickness, FLinearColor Color) {
	Canvas->K2_DrawLine(FVector2D(Pos.X, Pos.Y), FVector2D(Pos.X + Width, Pos.Y), Thickness, Color);
	Canvas->K2_DrawLine(FVector2D(Pos.X, Pos.Y), FVector2D(Pos.X, Pos.Y + Height), Thickness, Color);
	Canvas->K2_DrawLine(FVector2D(Pos.X + Width, Pos.Y), FVector2D(Pos.X + Width, Pos.Y + Height), Thickness, Color);
	Canvas->K2_DrawLine(FVector2D(Pos.X, Pos.Y + Height), FVector2D(Pos.X + Width, Pos.Y + Height), Thickness, Color);
}

int GetGameFps() {
    auto ScriptHelperClient = (UScriptHelperClient*)UScriptHelperClient::StaticClass();
    if (ScriptHelperClient) {
        float fps = ScriptHelperClient->GetFPS();
        return static_cast<int>(fps);
    }
    return 0;
}



void VectorAnglesRadar(Vector3 & forward, FVector & angles) {
 if (forward.X == 0.f && forward.Y == 0.f) {
  angles.X = forward.Z > 0.f ? -90.f : 90.f;
  angles.Y = 0.f;
 } else {
  angles.X = RAD2DEG(atan2(-forward.Z, forward.Magnitude(forward)));
  angles.Y = RAD2DEG(atan2(forward.Y, forward.X));
 }
 angles.Z = 0.f;
}
void DrawArrows(UCanvas* Canvas, FVector2D xy1, FVector2D xy2, FVector2D xy3 , float thickness, FLinearColor color) {
Canvas->K2_DrawLine(xy1, xy2, thickness ,color);
Canvas->K2_DrawLine(xy2, xy3, thickness ,color);
Canvas->K2_DrawLine(xy1, xy3, thickness ,color);
}
FVector WorldToRadar(float Yaw, FVector Origin, FVector LocalOrigin, float PosX, float PosY, Vector3 Size, bool & outbuff) {
 bool flag = false;
 double num = (double)Yaw;
 double num2 = num * 0.017453292519943295;
 float num3 = (float)std::cos(num2);
 float num4 = (float)std::sin(num2);
 float num5 = Origin.X - LocalOrigin.X;
 float num6 = Origin.Y - LocalOrigin.Y;
 struct FVector Xector;
 Xector.X = (num6 * num3 - num5 * num4) / 150.f;
 Xector.Y = (num5 * num3 + num6 * num4) / 150.f;
 struct FVector Xector2;
 Xector2.X = Xector.X + PosX + Size.X / 2.f;
 Xector2.Y = -Xector.Y + PosY + Size.Y / 2.f;
 bool flag2 = Xector2.X > PosX + Size.X;
 if (flag2) {
  Xector2.X = PosX + Size.X;
 } else {
  bool flag3 = Xector2.X < PosX;
  if (flag3) {
   Xector2.X = PosX;
  }
 }
 bool flag4 = Xector2.Y > PosY + Size.Y;
 if (flag4) {
  Xector2.Y = PosY + Size.Y;
 } else {
  bool flag5 = Xector2.Y < PosY;
  if (flag5) {
   Xector2.Y = PosY;
  }
 }
 bool flag6 = Xector2.Y == PosY || Xector2.X == PosX;
 if (flag6) {
  flag = true;
 }
 outbuff = flag;
 return Xector2;
}

void Box4Line(UCanvas* Canvas, float thicc, int x, int y, int w, int h, FLinearColor color) {
    int iw = w / 4;
    int ih = h / 4;
    // top
    Canvas->K2_DrawLine(FVector2D(x, y),FVector2D(x + iw, y), thicc, color);
    Canvas->K2_DrawLine(FVector2D(x + w - iw, y),FVector2D(x + w, y), thicc, color);
    Canvas->K2_DrawLine(FVector2D(x, y),FVector2D(x, y + ih), thicc, color);
    Canvas->K2_DrawLine(FVector2D(x + w - 1, y),FVector2D(x + w - 1, y + ih), thicc, color);;
    // bottom
    Canvas->K2_DrawLine(FVector2D(x, y + h),FVector2D(x + iw, y + h), thicc, color);
    Canvas->K2_DrawLine(FVector2D(x + w - iw, y + h),FVector2D(x + w, y + h), thicc, color);
    Canvas->K2_DrawLine(FVector2D(x, y + h - ih), FVector2D(x, y + h), thicc, color);
    Canvas->K2_DrawLine(FVector2D(x + w - 1, y + h - ih), FVector2D(x + w - 1, y + h), thicc, color);
}

void DrawCircleM(UCanvas* Canvas, int x, int y, int radius, FLinearColor OutlineColor) {
    float Step = 2.0f * M_PI / (float)radius;
    int Count = 0;
    FVector2D V[128];
    for (float a = 0; a < M_PI * 2.0; a += Step) {
        float X1 = radius * cos(a) + x;
        float Y1 = radius * sin(a) + y;
        float X2 = radius * cos(a + Step) + x;
        float Y2 = radius * sin(a + Step) + y;
        V[Count].X = X1;
        V[Count].Y = Y1;
        V[Count + 1].X = X2;
        V[Count + 1].Y = Y2;
        Canvas->K2_DrawLine(FVector2D(V[Count].X, V[Count].Y), FVector2D(X2, Y2), 1.0f, OutlineColor);
        Count += 2;
    }
}



void DrawFilledCircle(UCanvas* Canvas, FVector2D pos, float r, FLinearColor color)
{
	float smooth = 0.07f;

	double PII = 3.14159265359;
	int size = (int)(2.0f * PII / smooth) + 1;

	float angle = 0.0f;
	int i = 0;

	for (; angle < 2 * PII; angle += smooth, i++)
	{

		Canvas->K2_DrawLine(FVector2D{ pos.X, pos.Y }, FVector2D{ pos.X + cosf(angle) * r, pos.Y + sinf(angle) * r }, 1.0f, color);
	}
}


void DrawCircle(UCanvas* Canvas, float x, float y, float radius, int numsides, FLinearColor OutlineColor){
    float Step = M_PI * 2.0 / numsides;
	int Count = 0;
	FVector2D V[128];
	for (float a = 0; a < M_PI * 2.0; a += Step)
	{
		float X1 = radius * cos(a) + x;
		float Y1 = radius * sin(a) + y;
		float X2 = radius * cos(a + Step) + x;
		float Y2 = radius * sin(a + Step) + y;
		V[Count].X = X1;
		V[Count].Y = Y1;
		V[Count + 1].X = X2;
		V[Count + 1].Y = Y2;
		Canvas->K2_DrawLine(FVector2D(V[Count].X, V[Count].Y), FVector2D(X2, Y2), 1.f, OutlineColor);
	}
}
namespace GUI
{
namespace Colors
	{
	// ── Dark Slate Navy Theme ──────────────────────────────
FLinearColor Text{ 0.f, 0.f, 0.f, 1.f };
FLinearColor Text2{ 1.f, 1.f, 1.f, 1.f };
FLinearColor Text_Shadow{};
FLinearColor Text_Outline{};

FLinearColor Window_Background{0.f, 0.f, 0.f, 0.8f};
FLinearColor Window_Header{ 0.01f, 0.01f, 0.01f, 1.0f };
FLinearColor Window_Head{COLOR_WHITE};
FLinearColor Tab_Idle{0.04f, 0.04f, 0.04f, 8.0f};
FLinearColor Tab_Hovered{0.03f,0.03f,0.03f,1.0f};
FLinearColor Tab_Active{0.05f, 0.05f, 0.05f, 1.0f};
FLinearColor Tab_Accent{1.0f, 0.00f, 0.00f, 1.0f};
FLinearColor InputBox{0.10f,0.10f,0.10f,1.0f};
FLinearColor Checkbox_Idle{0.45f,0.45f,0.45f,1.0f};
FLinearColor Checkbox_Hovered{0.58f,0.58f,0.58f,1.0f};
FLinearColor Checkbox_Enabled{1.0f, 0.00f, 0.00f, 1.0f};
FLinearColor Combobox_Idle{0.10f,0.10f,0.10f,1.0f};
FLinearColor Combobox_Hovered{0.15f,0.15f,0.15f,1.0f};
FLinearColor Combobox_Elements{0.13f,0.13f,0.13f,1.0f};
FLinearColor Slider_Idle{0.10f,0.10f,0.10f,1.0f};
FLinearColor Slider_Hovered{0.15f,0.15f,0.15f,1.0f};
FLinearColor Slider_Progress{1.0f, 0.00f, 0.00f, 1.0f};
FLinearColor Slider_Button{0.62f,0.12f,0.82f,1.0f};
FLinearColor ColorPicker_Background{0.025f,0.025f,0.025f,1.0f};
const FLinearColor ACCENT{1.0f, 0.00f, 0.00f, 1.0f};
const FLinearColor BORDER{COLOR_BLACK};
const FLinearColor TAB_BORDER_OFF{COLOR_BLACK};
const FLinearColor TAB_BORDER_ON{COLOR_BLACK};
const FLinearColor DIM_TEXT{0.50f,0.50f,0.50f,1.0f};



	}

	namespace PostRenderer
	{
		struct DrawList
		{
			int type = -1; //1 = FilledRect, 2 = TextLeft, 3 = TextCenter, 4 = Draw_Line
			FVector2D pos;
			FVector2D size;
			FLinearColor color;
			char* name;
			bool outline;

			FVector2D from;
			FVector2D to;
			int thickness;
		};
		DrawList drawlist[128];

		void drawFilledRect(FVector2D pos, float w, float h, FLinearColor color)
		{
			for (int i = 0; i < 128; i++)
			{
				if (drawlist[i].type == -1)
				{
					drawlist[i].type = 1;
					drawlist[i].pos = pos;
					drawlist[i].size = FVector2D{ w, h };
					drawlist[i].color = color;
					return;
				}
			}
		}
		void TextLeft(char* name, FVector2D pos, FLinearColor color, bool outline)
		{
			for (int i = 0; i < 128; i++)
			{
				if (drawlist[i].type == -1)
				{
					drawlist[i].type = 2;
					drawlist[i].name = name;
					drawlist[i].pos = pos;
					drawlist[i].outline = outline;
					drawlist[i].color = color;
					return;
				}
			}
		}
		void TextCenter(char* name, FVector2D pos, FLinearColor color, bool outline)
		{
			for (int i = 0; i < 128; i++)
			{
				if (drawlist[i].type == -1)
				{
					drawlist[i].type = 3;
					drawlist[i].name = name;
					drawlist[i].pos = pos;
					drawlist[i].outline = outline;
					drawlist[i].color = color;
					return;
				}
			}
		}
		void Draw_Line(FVector2D from, FVector2D to, int thickness, FLinearColor color)
		{
			for (int i = 0; i < 128; i++)
			{
				if (drawlist[i].type == -1)
				{
					drawlist[i].type = 4;
					drawlist[i].from = from;
					drawlist[i].to = to;
					drawlist[i].thickness = thickness;
					drawlist[i].color = color;
					return;
				}
			}
		}
	}

	UCanvas* Canvas;
	void* font;

	bool hover_element = false;
	FVector2D menu_pos = FVector2D{ 0, 0 };
	float offset_x = 0.0f;
	float offset_y = 0.0f;

	FVector2D first_element_pos = FVector2D{ 0, 0 };

	FVector2D last_element_pos = FVector2D{ 0, 0 };
	FVector2D last_element_size = FVector2D{ 0, 0 };

	int current_element = -1;
	FVector2D current_element_pos = FVector2D{ 0, 0 };
	FVector2D current_element_size = FVector2D{ 0, 0 };
	int elements_count = 0;

	bool sameLine = false;

    bool pushY = false;
	float pushYvalue = 0.0f;
	bool mouseDown[5];
	bool mouseDownAlready[256];
    bool KeysDown[512];
    FVector2D MousePos = {0,0};
    bool MouseDown = false;
	bool keysDownAlready[256];


    bool IsMouseClicked(int button, int element_id, bool repeat)
    {
        if (MouseDown)
        {
            if (!mouseDownAlready[element_id])
            {
                mouseDownAlready[element_id] = true;
                return true;
            }
            if (repeat)
                return true;
        }
        else
        {
            mouseDownAlready[element_id] = false;
        }
        return false;
	}

	void SetupCanvas(UCanvas* _Canvas, void* _font)
	{
		Canvas = _Canvas;
		font = _font;
	}

	FVector2D CursorPos()
    {
        return MousePos;
	}
	
	bool MouseInZone(FVector2D pos, FVector2D size)
    {
        FVector2D cursor_pos = CursorPos();
        if (cursor_pos.X > pos.X && cursor_pos.Y > pos.Y)
            if (cursor_pos.X < pos.X + size.X && cursor_pos.Y < pos.Y + size.Y)
                return true;

        return false;
	}
	
	void 音量键() {
     for (auto& key_queue : g_KeyEventQueues) {
        if (key_queue.second.empty())
            continue;

        // 获取当前按键的事件
        int action = key_queue.second.front();
        key_queue.second.pop(); // 移除处理过的事件

        // 更新 KeysDown 数组
        bool wasDown = KeysDown[key_queue.first];
        KeysDown[key_queue.first] = (action == AKEY_EVENT_ACTION_DOWN);

        // 如果当前按键是按下状态且之前没有按下，切换窗口状态
        if (KeysDown[key_queue.first] && !wasDown) {
         窗口 = !窗口;       
        }
    }
}
	
	void Draw_Cursor(bool toogle)
	{
		if (toogle)
		{
			FVector2D cursorPos = CursorPos();
		    Canvas->K2_DrawLine(FVector2D{ cursorPos.X, cursorPos.Y }, FVector2D{ cursorPos.X + 35, cursorPos.Y + 10 }, 1, FLinearColor{ 0.30f, 0.30f, 0.80f, 1.0f });


			int x = 35;
			int y = 10;
			while (y != 30) //20 steps
			{
				x -= 1; if (x < 15) x = 15;
				y += 1; if (y > 30) y = 30;

				Canvas->K2_DrawLine(FVector2D{ cursorPos.X, cursorPos.Y }, FVector2D{ cursorPos.X + x, cursorPos.Y + y }, 1, FLinearColor{ 0.30f, 0.30f, 0.80f, 1.0f });
			}

			Canvas->K2_DrawLine(FVector2D{ cursorPos.X, cursorPos.Y }, FVector2D{ cursorPos.X + 15, cursorPos.Y + 30 }, 1, FLinearColor{ 0.30f, 0.30f, 0.80f, 1.0f });
			Canvas->K2_DrawLine(FVector2D{ cursorPos.X + 35, cursorPos.Y + 10 }, FVector2D{ cursorPos.X + 15, cursorPos.Y + 30 }, 1, FLinearColor{ 0.30f, 0.30f, 0.80f, 1.0f });
		}
	}

	void SameLine()
	{
		sameLine = true;
	}
	
	void PushNextElementY(float y, bool from_last_element = true)
	{
		pushY = true;
		if (from_last_element)
			pushYvalue = last_element_pos.Y + last_element_size.Y + y;
		else
			pushYvalue = y;
	}
	void NextColumn(float x)
	{
		offset_x = x;
		PushNextElementY(first_element_pos.Y, false);
	}
	void 下一行(float y)
	{
		offset_y = y;
		PushNextElementY(first_element_pos.X, false);
	}
	void ClearFirstPos()
	{
		first_element_pos = FVector2D{ 0, 0 };
	}

	void TextLeft(const char* name, FVector2D pos, FLinearColor color, bool outline)
	{
		int length = strlen(name) + 1;
		Canvas->K2_DrawText(tslFont, FString(name),pos, color, false, Colors::Text_Shadow, FVector2D{ pos.X + 1, pos.Y + 1 }, false, true, true, Colors::Text_Outline);    
	}
	void TextCenter(const char* name, FVector2D pos, FLinearColor color, bool outline)
	{
		int length = strlen(name) + 1;
		Canvas->K2_DrawText(tslFont, FString(name),pos, color, false, Colors::Text_Shadow, FVector2D{ pos.X + 1, pos.Y + 1 }, true, true, true, Colors::Text_Outline);
	}
	
void K2_DrawLine(FVector2D start, FVector2D end, float Thickness, FLinearColor col) {
		DrawLine(GUI::Canvas,FVector2D{ start.X, start.Y }, FVector2D{ end.X, end.Y }, Thickness, col);
    //你的实现代码
}

#define PI 3.141592653589793238
void drawHexagonStar(float x, float y, float radius, float DeltaTime, FLinearColor color){
  const int numPoints = 6; // 六角星有6个顶点
  FVector2D center(x, y);
  FVector2D points[numPoints];
  for (int i = 0; i < numPoints; i++)
  {
    float angle = DeltaTime + 2 * PI * i / numPoints;
    points[i] = FVector2D(center.X + radius * cos(angle), center.Y + radius * sin(angle));
  }
  // 绘制两个大三角形
  K2_DrawLine(points[0], points[2], 1.0f, color);
  K2_DrawLine(points[2], points[4], 1.0f, color);
  K2_DrawLine(points[4], points[0], 1.0f, color);
  K2_DrawLine(points[1], points[3], 1.0f, color);
  K2_DrawLine(points[3], points[5], 1.0f, color);
  K2_DrawLine(points[5], points[1], 1.0f, color);
}

	void GetColor(FLinearColor* color, float* r, float* g, float* b, float* a)
	{
		*r = color->R;
		*g = color->G;
		*b = color->B;
		*a = color->A;
	}
	UINT32 GetColorUINT(int r, int g, int b, int a)
	{
		UINT32 result = (BYTE(a) << 24) + (BYTE(r) << 16) + (BYTE(g) << 8) + BYTE(b);
		return result;
	}

	void Draw_Line(FVector2D from, FVector2D to, int thickness, FLinearColor color)
	{
		Canvas->K2_DrawLine(FVector2D{ from.X, from.Y }, FVector2D{ to.X, to.Y }, thickness, color);
	}
	void drawFilledRect(FVector2D initial_pos, float w, float h, FLinearColor color)
	{
		for (float i = 0.0f; i < h; i += 1.0f)
		Canvas->K2_DrawLine(FVector2D{ initial_pos.X, initial_pos.Y + i }, FVector2D{ initial_pos.X + w, initial_pos.Y + i }, 1.0f, color);
	}
	
	
    // Optimized filled circle.
    // Old code: ~628 Canvas draw calls per circle.
    // New code: 24 draw calls per circle.
    void DrawFilledCircle(FVector2D pos, float r, FLinearColor color)
    {
        constexpr int SEGMENTS = 24;
        constexpr float TWO_PI = 6.28318530718f;
        const float step = TWO_PI / static_cast<float>(SEGMENTS);

        for (int i = 0; i < SEGMENTS; ++i)
        {
            const float angle = step * static_cast<float>(i);

            Draw_Line(
                FVector2D{ pos.X, pos.Y },
                FVector2D{
                    pos.X + cosf(angle) * r,
                    pos.Y + sinf(angle) * r
                },
                1.0f,
                color
            );
        }
    }
    /*
	void DrawFilledCircle(UCanvas* Canvas, int x, int y, int radius, int numsides, FLinearColor OutlineColor, FLinearColor FillColor){
    float Step = M_PI * 2.0 / numsides;
    FVector2D Center(x, y);
    FVector2D V[128];
    for (int i = 0; i < numsides; ++i)
    {
        // Calculate vertices for the i-th triangle
        float Angle = Step * i;
        V[i].X = Center.X + radius * cos(Angle);
        V[i].Y = Center.Y + radius * sin(Angle);

        // Draw lines from the center to the vertices
        Canvas->K2_DrawLine(Center, V[i], 1.f, FillColor);
    }*/
		void NewFrame(int screen_width, int screen_height)
    {
        for (auto& key_queue : g_KeyEventQueues)
        {
            if (key_queue.second.empty())
                continue;
            KeysDown[key_queue.first] = (key_queue.second.front() == AKEY_EVENT_ACTION_DOWN);
            key_queue.second.pop();
        }
    }
	void DrawCircle(FVector2D pos, int radius, int numSides, FLinearColor Color)
	{
		float Pl = 3.1415927f;

		float Step = Pl * 2.0 / numSides;
		int Count = 0;
		FVector2D V[128];
		for (float a = 0; a < Pl * 2.0; a += Step) {
			float X1 = radius * cos(a) + pos.X;
			float Y1 = radius * sin(a) + pos.Y;
			float X2 = radius * cos(a + Step) + pos.X;
			float Y2 = radius * sin(a + Step) + pos.Y;
			V[Count].X = X1;
			V[Count].Y = Y1;
			V[Count + 1].X = X2;
			V[Count + 1].Y = Y2;
			//Draw_Line(FVector2D{ pos.X, pos.Y }, FVector2D{ X2, Y2 }, 1.0f, Color); // Points from Centre to ends of circle
			Draw_Line(FVector2D{ V[Count].X, V[Count].Y }, FVector2D{ X2, Y2 }, 1.0f, Color);// Circle Around
		}
	}
	
	FVector2D dragPos;
	FVector2D movePos;
	FVector2D origPos;
    
#include <cmath>

// نقطة خلفية مع إزاحة نبض خاصة
struct SnowPoint {
    float offset; // لكل نقطة فارق توقيت نبض
};

#define GRID_ROWS 7
#define GRID_COLS 12
SnowPoint snowGrid[GRID_ROWS][GRID_COLS];
bool gridInitialized = false;
float timeCounter = 0.0f;

// ─── زوايا L-shape زخرفية ───────────────────────────────────────
void DrawCornerBrackets(FVector2D pos, FVector2D size, float len, float thick, FLinearColor col)
{
    float x = pos.X, y = pos.Y, w = size.X, h = size.Y;
    // أعلى يسار
    Draw_Line({x,     y    }, {x+len, y    }, thick, col);
    Draw_Line({x,     y    }, {x,     y+len}, thick, col);
    // أعلى يمين
    Draw_Line({x+w,   y    }, {x+w-len, y  }, thick, col);
    Draw_Line({x+w,   y    }, {x+w,   y+len}, thick, col);
    // أسفل يسار
    Draw_Line({x,     y+h  }, {x+len, y+h  }, thick, col);
    Draw_Line({x,     y+h  }, {x,   y+h-len}, thick, col);
    // أسفل يمين
    Draw_Line({x+w,   y+h  }, {x+w-len,y+h }, thick, col);
    Draw_Line({x+w,   y+h  }, {x+w, y+h-len}, thick, col);
}

// ─── خط مضيء بتدرج (glow line) ──────────────────────────────────
void DrawGlowLine(FVector2D from, FVector2D to, FLinearColor col, float alpha)
{
    FLinearColor c1 = {col.R, col.G, col.B, alpha * 0.08f};
    FLinearColor c2 = {col.R, col.G, col.B, alpha * 0.22f};
    FLinearColor c3 = {col.R, col.G, col.B, alpha};
    // طبقات متعددة لتأثير التوهج
    Draw_Line({from.X-1,from.Y-1},{to.X-1,to.Y-1}, 4, c1);
    Draw_Line({from.X,  from.Y  },{to.X,  to.Y  }, 3, c2);
    Draw_Line({from.X+1,from.Y+1},{to.X+1,to.Y+1}, 2, c3);
    Draw_Line({from.X,  from.Y  },{to.X,  to.Y  }, 1, {col.R,col.G,col.B,1.0f});
}

bool Window(
    char* name,
    FVector2D* pos,
    FVector2D size,
    bool& isOpen,
    float& tempValue)
{
    elements_count = 0;

    // =====================================================
    // LOGO SETTINGS
    // =====================================================

    const float logoRadius  = 30.0f;
    const float closedHeight = 106.0f;

    // =====================================================
    // LOGO CENTER
    // =====================================================

    FVector2D logoCenter =
    {
        pos->X + size.X * 0.5f,
        pos->Y + closedHeight * 0.5f
    };

    // =====================================================
    // MOUSE / HOVER
    // =====================================================

    FVector2D mousePos = CursorPos();

    bool isHovered = false;

    if (isOpen)
    {
        // عند فتح المنيو: المنطقة كاملة
        isHovered = MouseInZone(
            FVector2D{
                pos->X,
                pos->Y
            },
            size
        );
    }
    else
    {
        // عند إغلاق المنيو:
        // الضغط داخل الدائرة فقط

        float dx =
            mousePos.X - logoCenter.X;

        float dy =
            mousePos.Y - logoCenter.Y;

        float distanceSquared =
            (dx * dx) + (dy * dy);

        isHovered =
            distanceSquared <=
            (logoRadius * logoRadius);
    }

    // =====================================================
    // RESET CURRENT ELEMENT
    // =====================================================

    if (current_element != -1 && !MouseDown)
        current_element = -1;

    // =====================================================
    // WINDOW CLICK / DRAG
    // =====================================================

    if (hover_element && MouseDown)
    {
    }
    else if ((isHovered || dragPos.X != 0) && !hover_element)
    {
        if (IsMouseClicked(
                0,
                elements_count,
                true))
        {
            FVector2D cursorPos = CursorPos();

            cursorPos.X -= size.X;
            cursorPos.Y -= size.Y;

            if (dragPos.X == 0)
            {
                dragPos.X =
                    cursorPos.X - pos->X;

                dragPos.Y =
                    cursorPos.Y - pos->Y;

                origPos =
                {
                    cursorPos.X - dragPos.X,
                    cursorPos.Y - dragPos.Y
                };

                // =================================================
                // TITLE / LOGO CLICK
                // =================================================

                bool isClickedTitle = false;

                if (isOpen)
                {
                    isClickedTitle =
                        MouseInZone(
                            FVector2D{
                                pos->X,
                                pos->Y
                            },
                            FVector2D{
                                size.X,
                                42.0f
                            }
                        );
                }
                else
                {
                    // اللوجو فقط
                    isClickedTitle = isHovered;
                }

                tempValue =
                    isClickedTitle
                    ? 1.0f
                    : 0.0f;
            }

            // =================================================
            // DRAG POSITION
            // =================================================

            pos->X =
                cursorPos.X - dragPos.X;

            pos->Y =
                cursorPos.Y - dragPos.Y;
        }
        else
        {
            dragPos =
                FVector2D{
                    0,
                    0
                };

            // =================================================
            // TOGGLE WINDOW
            // =================================================

            if (tempValue &&
                abs(origPos.X - pos->X) <= 10 &&
                abs(origPos.Y - pos->Y) <= 10)
            {
                tempValue = 0;

                isOpen = !isOpen;
            }
        }
    }
    else
    {
        hover_element = false;
    }

    // =====================================================
    // RESET MENU POSITIONS
    // =====================================================

    offset_x = 0.0f;
    offset_y = 0.0f;

    menu_pos =
        FVector2D{
            pos->X,
            pos->Y
        };

    first_element_pos =
        FVector2D{
            0,
            0
        };

    current_element_pos =
        FVector2D{
            0,
            0
        };

    current_element_size =
        FVector2D{
            0,
            0
        };

    // =====================================================
    // ANIMATION
    // =====================================================

    timeCounter += 0.018f;

    float pulse =
        (sinf(timeCounter) + 1.0f) * 0.5f;

    float pulse2 =
        (sinf(timeCounter * 1.7f) + 1.0f) * 0.5f;

    // =====================================================
    // HEADER HEIGHT
    // =====================================================

    float headerHeight =
        isOpen
        ? 42.0f
        : closedHeight;

    // =====================================================
    // OPEN WINDOW
    // =====================================================

    if (isOpen)
    {
        // =================================================
        // MAIN BACKGROUND
        // =================================================

        DrawBox(
            Canvas,
            {
                pos->X,
                pos->Y + headerHeight
            },
            {
                size.X,
                size.Y - headerHeight
            },
            Colors::Window_Background
        );

        // =================================================
        // HEADER
        // =================================================

        DrawBox(
            Canvas,
            {
                pos->X,
                pos->Y
            },
            {
                size.X,
                headerHeight
            },
            Colors::Window_Header
        );

        // =================================================
        // OUTER BORDER
        // =================================================

        DrawRectangle(
            Canvas,
            {
                pos->X,
                pos->Y
            },
            size.X,
            size.Y,
            1.0f,
            Colors::Window_Head
        );

        // =================================================
        // L-SHAPE CORNERS
        // =================================================

        float bAlpha =
            0.55f +
            0.45f * pulse;

        FLinearColor bc =
        {
            Colors::ACCENT.R,
            Colors::ACCENT.G,
            Colors::ACCENT.B,
            bAlpha
        };

        DrawCornerBrackets(
            {
                pos->X,
                pos->Y
            },
            {
                size.X,
                size.Y
            },
            120.0f,
            2.5f,
            bc
        );

        // =================================================
        // HEADER SEPARATOR
        // =================================================

        Draw_Line(
            {
                pos->X,
                pos->Y + headerHeight
            },
            {
                pos->X + size.X,
                pos->Y + headerHeight
            },
            1.0f,
            Colors::Window_Head
        );

        // =================================================
        // ACCENT GLOW LINE
        // =================================================

        float aW =
            size.X * 0.38f;

        float aX =
            pos->X +
            (size.X - aW) * 0.5f;

        float aAlpha =
            0.50f +
            0.50f * pulse;

        DrawGlowLine(
            {
                aX,
                pos->Y + headerHeight - 1.5f
            },
            {
                aX + aW,
                pos->Y + headerHeight - 1.5f
            },
            Colors::ACCENT,
            aAlpha
        );

        // =================================================
        // LEFT ROTATING TRIANGLE
        // =================================================

        float sx =
            pos->X + 22.0f;

        float sy =
            pos->Y +
            headerHeight * 0.5f;

        float sr =
            5.5f +
            1.0f * pulse;

        float angle =
            timeCounter;

        FLinearColor sCol =
        {
            Colors::ACCENT.R,
            Colors::ACCENT.G,
            Colors::ACCENT.B,
            0.40f +
            0.40f * pulse
        };

        FVector2D p1 =
        {
            sx + sr * cosf(angle),
            sy + sr * sinf(angle)
        };

        FVector2D p2 =
        {
            sx + sr * cosf(angle + 2.094f),
            sy + sr * sinf(angle + 2.094f)
        };

        FVector2D p3 =
        {
            sx + sr * cosf(angle + 4.189f),
            sy + sr * sinf(angle + 4.189f)
        };

        Draw_Line(
            p1,
            p2,
            1.5f,
            sCol
        );

        Draw_Line(
            p2,
            p3,
            1.5f,
            sCol
        );

        Draw_Line(
            p3,
            p1,
            1.5f,
            sCol
        );

        // =================================================
        // RIGHT ROTATING TRIANGLE
        // =================================================

        float sx2 =
            pos->X +
            size.X -
            22.0f;

        float sy2 =
            pos->Y +
            headerHeight * 0.5f;

        float sr2 =
            5.5f +
            1.0f * pulse2;

        float angle2 =
            -timeCounter;

        FLinearColor sCol2 =
        {
            Colors::ACCENT.R,
            Colors::ACCENT.G,
            Colors::ACCENT.B,
            0.40f +
            0.40f * pulse2
        };

        FVector2D q1 =
        {
            sx2 + sr2 * cosf(angle2),
            sy2 + sr2 * sinf(angle2)
        };

        FVector2D q2 =
        {
            sx2 + sr2 * cosf(angle2 + 2.094f),
            sy2 + sr2 * sinf(angle2 + 2.094f)
        };

        FVector2D q3 =
        {
            sx2 + sr2 * cosf(angle2 + 4.189f),
            sy2 + sr2 * sinf(angle2 + 4.189f)
        };

        Draw_Line(
            q1,
            q2,
            1.5f,
            sCol2
        );

        Draw_Line(
            q2,
            q3,
            1.5f,
            sCol2
        );

        Draw_Line(
            q3,
            q1,
            1.5f,
            sCol2
        );

        // =================================================
        // OFFSET
        // =================================================

        offset_y += headerHeight;

        // =================================================
        // TITLE
        // =================================================

        FVector2D titlePos =
        {
            pos->X +
            size.X / 2.0f,

            pos->Y +
            headerHeight / 2.0f
        };

        TextCenter(
            name,
            titlePos,
            Colors::Text2,
            false
        );
    }

    // =====================================================
    // CLOSED LOGO
    // =====================================================

    else
    {
        // =================================================
        // LOGO CENTER
        // =================================================

        FVector2D center =
        {
            pos->X +
            size.X * 0.5f,

            pos->Y +
            closedHeight * 0.5f
        };

        // =================================================
        // COLORS
        // =================================================

        // أسود شفاف
        FLinearColor logoBG =
        {
            0.0f,
            0.0f,
            0.0f,
            0.82f
        };

        // أبيض
        FLinearColor white =
        {
            1.0f,
            1.0f,
            1.0f,
            0.98f
        };

        // =================================================
        // FILLED CIRCLE
        // =================================================

        for (int y = -(int)logoRadius;
             y <= (int)logoRadius;
             y++)
        {
            float yy =
                (float)y;

            float xx =
                sqrtf(
                    (logoRadius * logoRadius) -
                    (yy * yy)
                );

            Draw_Line(
                {
                    center.X - xx,
                    center.Y + yy
                },
                {
                    center.X + xx,
                    center.Y + yy
                },
                1.0f,
                logoBG
            );
        }

        // =================================================
        // CIRCLE OUTLINE
        // =================================================

        const int circleSegments = 64;

        for (int i = 0;
             i < circleSegments;
             i++)
        {
            float a1 =
                (2.0f *
                 3.14159265f *
                 i) /
                circleSegments;

            float a2 =
                (2.0f *
                 3.14159265f *
                 (i + 1)) /
                circleSegments;

            FVector2D c1 =
            {
                center.X +
                logoRadius * cosf(a1),

                center.Y +
                logoRadius * sinf(a1)
            };

            FVector2D c2 =
            {
                center.X +
                logoRadius * cosf(a2),

                center.Y +
                logoRadius * sinf(a2)
            };

            Draw_Line(
                c1,
                c2,
                2.0f,
                white
            );
        }

        // =================================================
        // ⚔ CROSSED SWORDS
        // =================================================

        float swordSize =
            16.0f;

        float swordWidth =
            3.0f;

        // =================================================
        // SWORD 1 \
        // =================================================

        // النصل
        Draw_Line(
            {
                center.X - swordSize,
                center.Y - swordSize
            },
            {
                center.X + swordSize,
                center.Y + swordSize
            },
            swordWidth,
            white
        );

        // واقي السيف
        Draw_Line(
            {
                center.X - 10.0f,
                center.Y - 4.0f
            },
            {
                center.X - 4.0f,
                center.Y - 10.0f
            },
            3.0f,
            white
        );

        // المقبض
        Draw_Line(
            {
                center.X - 16.0f,
                center.Y - 16.0f
            },
            {
                center.X - 21.0f,
                center.Y - 21.0f
            },
            3.0f,
            white
        );

        // نهاية المقبض
        Draw_Line(
            {
                center.X - 23.0f,
                center.Y - 19.0f
            },
            {
                center.X - 19.0f,
                center.Y - 23.0f
            },
            3.0f,
            white
        );

        // =================================================
        // SWORD 2 /
        // =================================================

        // النصل
        Draw_Line(
            {
                center.X + swordSize,
                center.Y - swordSize
            },
            {
                center.X - swordSize,
                center.Y + swordSize
            },
            swordWidth,
            white
        );

        // واقي السيف
        Draw_Line(
            {
                center.X + 10.0f,
                center.Y - 4.0f
            },
            {
                center.X + 4.0f,
                center.Y - 10.0f
            },
            3.0f,
            white
        );

        // المقبض
        Draw_Line(
            {
                center.X + 16.0f,
                center.Y - 16.0f
            },
            {
                center.X + 21.0f,
                center.Y - 21.0f
            },
            3.0f,
            white
        );

        // نهاية المقبض
        Draw_Line(
            {
                center.X + 23.0f,
                center.Y - 19.0f
            },
            {
                center.X + 19.0f,
                center.Y - 23.0f
            },
            3.0f,
            white
        );
    }

    // =====================================================
    // RETURN
    // =====================================================

    return isOpen;
}

	void Text(const char* text, bool center = false, bool outline = false)
	{
		elements_count++;

		float size = 12.5;
		FVector2D padding = FVector2D{ 0, 20 };
		FVector2D pos = FVector2D{ menu_pos.X + padding.X + offset_x, menu_pos.Y + padding.Y + offset_y };
		if (sameLine)
		{
			pos.X = last_element_pos.X + last_element_size.X + padding.X;
			pos.Y = last_element_pos.Y;
		}
		if (pushY)
		{
			pos.Y = pushYvalue;
			pushY = false;
			pushYvalue = 0.0f;
			offset_y = pos.Y - menu_pos.Y;
		}

		if (!sameLine)
			offset_y += size + padding.Y;

		//Text
		FVector2D textPos = FVector2D{ pos.X + 5.0f, pos.Y + size / 2 };
		if (center)
			TextCenter(text, textPos, Colors::DIM_TEXT, outline);
		else
			TextLeft(text, textPos, Colors::DIM_TEXT, outline);

		sameLine = false;
		last_element_pos = pos;
		//last_element_size = size;
		if (first_element_pos.X == 0.0f)
			first_element_pos = pos;
	}
	void TextLogin(const char* text, bool center = false, bool outline = false)
	{
		elements_count++;

		float size = 12.5;
		FVector2D padding = FVector2D{ 20, 20 };
		FVector2D pos = FVector2D{ menu_pos.X + padding.X + offset_x, menu_pos.Y + padding.Y + offset_y };
		if (sameLine)
		{
			pos.X = last_element_pos.X + last_element_size.X + padding.X;
			pos.Y = last_element_pos.Y;
		}
		if (pushY)
		{
			pos.Y = pushYvalue;
			pushY = false;
			pushYvalue = 0.0f;
			offset_y = pos.Y - menu_pos.Y;
		}

		if (!sameLine)
			offset_y += size + padding.Y;

		//Text
		FVector2D textPos2 = FVector2D{ pos.X + 5.0f, pos.Y + size / 2 };
		if (center)
			TextCenter(text, textPos2, Colors::Text2, outline);
		else
			TextLeft(text, textPos2, Colors::Text2, outline);

		sameLine = false;
		last_element_pos = pos;
		//last_element_size = size;
		if (first_element_pos.X == 0.0f)
			first_element_pos = pos;
	}
	
	
void DrawTabsBackground(FVector2D final_pos, FVector2D tab_size, int total_tabs, float padding_x)
{
    // 1. حساب العرض الكلي للتابات مع الفراغات بينها
    float total_width = (tab_size.X * total_tabs) + (padding_x * (total_tabs - 1));
    
    // 2. تحديد حجم الإطار الخارجي (زيادة مساحة صغيرة حول التابات)
    float bg_padding_offset = 10.0f; 
    FVector2D bg_size = FVector2D{ total_width + (bg_padding_offset * 2), tab_size.Y + (bg_padding_offset * 2) };

    // ضبط الموضع ليتوسط التابات تماماً بناءً على الموضع الممرر
    FVector2D bg_pos = FVector2D{ final_pos.X - bg_padding_offset, final_pos.Y - bg_padding_offset };

    // 3. تحديد الألوان (رصاصي فيراني داكن وحدود سوداء صريحة)
    FLinearColor charcoal_gray = FLinearColor{ 0.03f, 0.03f, 0.03f, 8.0f }; 
    FLinearColor pure_black    = FLinearColor{ 0.00f, 0.00f, 0.00f, 1.0f }; 

    // 4. عملية الرسم
    drawFilledRect(bg_pos, bg_size.X, bg_size.Y, charcoal_gray);
    DrawRectangle(Canvas, bg_pos, bg_size.X, bg_size.Y, 1.5f, pure_black);
}


	
	
bool ButtonTab(const char* name, FVector2D size, bool active)
{
    elements_count++;

    FVector2D padding = FVector2D{ 20, 20 };
    FVector2D pos = FVector2D{ menu_pos.X + padding.X + offset_x, menu_pos.Y + padding.Y + offset_y };

    if (sameLine)
    {
        pos.X = last_element_pos.X + last_element_size.X + padding.X;
        pos.Y = last_element_pos.Y;
    }

    if (pushY)
    {
        pos.Y = pushYvalue;
        pushY = false;
        pushYvalue = 0.0f;
        offset_y = pos.Y - menu_pos.Y;
    }

    bool isHovered = MouseInZone(FVector2D{ pos.X, pos.Y }, size);

    if (active)
    {
        drawFilledRect(
            FVector2D{ pos.X, pos.Y },
            size.X,
            size.Y,
            Colors::Tab_Active
        );

        DrawRectangle(
            Canvas,
            FVector2D{ pos.X, pos.Y },
            size.X,
            size.Y,
            2.0f,
            Colors::TAB_BORDER_ON
        );

        DrawGlowLine(
            { pos.X + 6.0f, pos.Y + size.Y - 0.5f },
            { pos.X + size.X - 6.0f, pos.Y + size.Y - 0.5f },
            Colors::ACCENT,
            0.90f
        );

        float pulse = (sinf(timeCounter * 2.0f) + 1.0f) * 0.5f;
        float bAlpha = 0.55f + 0.45f * pulse;

        FLinearColor bc = {
            Colors::ACCENT.R,
            Colors::ACCENT.G,
            Colors::ACCENT.B,
            bAlpha
        };

        float offset = 1.0f;

        FVector2D bracketPos = {
            pos.X - offset,
            pos.Y - offset
        };

        FVector2D bracketSize = {
            size.X + (offset * 2.0f),
            size.Y + (offset * 2.0f)
        };

         DrawCornerBrackets(
             bracketPos,
             bracketSize,
             18.0f,
             2.0f,
             bc
         );
    }
    else if (isHovered)
    {
        drawFilledRect(
            FVector2D{ pos.X, pos.Y },
            size.X,
            size.Y,
            Colors::Tab_Hovered
        );

        DrawRectangle(
            Canvas,
            FVector2D{ pos.X, pos.Y },
            size.X,
            size.Y,
            2.0f,
            Colors::TAB_BORDER_OFF
        );

        hover_element = true;
    }
    else
    {
        drawFilledRect(
            FVector2D{ pos.X, pos.Y },
            size.X,
            size.Y,
            Colors::Tab_Idle
        );

        DrawRectangle(
            Canvas,
            FVector2D{ pos.X, pos.Y },
            size.X,
            size.Y,
            2.0f,
            Colors::TAB_BORDER_OFF
        );
    }

    if (!sameLine)
        offset_y += size.Y + padding.Y;

    FVector2D textPos = {
        pos.X + size.X / 2.0f,
        pos.Y + size.Y / 2.0f
    };

    FLinearColor tCol = active
        ? Colors::Text2
        : Colors::DIM_TEXT;

    TextCenter(
        name,
        textPos,
        tCol,
        false
    );

    sameLine = false;

    last_element_pos = pos;
    last_element_size = size;

    if (first_element_pos.X == 0.0f)
        first_element_pos = pos;

    if (isHovered && IsMouseClicked(0, elements_count, false))
        return true;

    return false;
}


	
	bool ButtonTab2(const char* name, FVector2D size, bool active)
	{
		elements_count++;

		FVector2D padding = FVector2D{ 20, 0 };
		FVector2D pos = FVector2D{ menu_pos.X + padding.X + offset_x, menu_pos.Y + padding.Y + offset_y };
		if (sameLine)
		{
			pos.X = last_element_pos.X + last_element_size.X + padding.X;
			pos.Y = last_element_pos.Y;
		}
		if (pushY)
		{
			pos.Y = pushYvalue;
			pushY = false;{}
			pushYvalue = 0.0f;
			offset_y = pos.Y - menu_pos.Y;
		}
		bool isHovered = MouseInZone(FVector2D{ pos.X, pos.Y }, size);

		 //Bg
        if (active)
        {
            drawFilledRect(FVector2D{pos.X, pos.Y}, size.X, size.Y, Colors::Tab_Active);
            DrawRectangle(Canvas, FVector2D{pos.X, pos.Y}, size.X, size.Y, 1.5f, Colors::TAB_BORDER_ON);
            Draw_Line(FVector2D{pos.X+4.f,pos.Y+size.Y-1.f},
                      FVector2D{pos.X+size.X-4.f,pos.Y+size.Y-1.f}, 2.f, Colors::Tab_Accent);
        }
        else if (isHovered)
        {
            drawFilledRect(FVector2D{pos.X, pos.Y}, size.X, size.Y, Colors::Tab_Hovered);
            DrawRectangle(Canvas, FVector2D{pos.X, pos.Y}, size.X, size.Y, 1.0f, Colors::TAB_BORDER_OFF);
            hover_element = true;
        }
        else
        {
            drawFilledRect(FVector2D{pos.X, pos.Y}, size.X, size.Y, Colors::Tab_Idle);
            DrawRectangle(Canvas, FVector2D{pos.X, pos.Y}, size.X, size.Y, 1.0f, Colors::TAB_BORDER_OFF);
		}

		if (!sameLine)
			offset_y += size.Y + padding.Y;

		//Text
		FVector2D textPos = FVector2D{ pos.X + size.X / 2.0f, pos.Y + size.Y / 2.0f };
		FLinearColor tCol2 = active ? Colors::Text2 : Colors::DIM_TEXT;
		TextCenter(name, textPos, tCol2, false);

		sameLine = false;
		last_element_pos = pos;
		last_element_size = size;
		if (first_element_pos.X == 0.0f)
			first_element_pos = pos;

		if (isHovered && IsMouseClicked(0, elements_count, false))
			return true;

		return false;
	}
	
bool Button(const char* name, FVector2D size, FLinearColor customColor = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f))
{
    elements_count++;

    const float paddingX = 10.0f;
    const float paddingY = 10.0f;

    FVector2D pos = FVector2D{
        menu_pos.X + paddingX + offset_x,
        menu_pos.Y + paddingY + offset_y
    };

    if (sameLine)
    {
        pos.X = last_element_pos.X + last_element_size.X + 10.0f;
        pos.Y = last_element_pos.Y;
    }

    bool isHovered = MouseInZone(pos, size);

    FLinearColor bgColor =
        FLinearColor(0.16f, 0.16f, 0.16f, 1.0f);

    FLinearColor borderColor =
        FLinearColor(0.26f, 0.26f, 0.26f, 1.0f);

    // النص أبيض
    FLinearColor textColor =
        FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);

    if (isHovered)
    {
        // الحواف بنفسجي مثل السويتش
        borderColor =
            FLinearColor(1.0f, 0.0f, 0.0f, 1.0f);

        bgColor.R += 0.03f;
        bgColor.G += 0.03f;
        bgColor.B += 0.03f;

        hover_element = true;
    }

    DrawRoundRect(
        Canvas,
        pos,
        size.X,
        size.Y,
        4.0f,
        borderColor
    );

    FVector2D innerPos = FVector2D{
        pos.X + 1.0f,
        pos.Y + 1.0f
    };

    FVector2D innerSize = FVector2D{
        size.X - 2.0f,
        size.Y - 2.0f
    };

    DrawRoundRect(
        Canvas,
        innerPos,
        innerSize.X,
        innerSize.Y,
        3.0f,
        bgColor
    );

    FVector2D textPos = FVector2D{
        pos.X + (size.X / 10.0f),
        pos.Y + (size.Y / 2.0f)
    };

    Canvas->K2_DrawText(
        tslFont,
        name,
        textPos,
        textColor,
        1.0f,
        {},
        {},
        false,
        true,
        false,
        {}
    );

    if (!sameLine)
        offset_y += size.Y + paddingY;

    sameLine = false;

    last_element_pos = pos;
    last_element_size = size;

    if (first_element_pos.X == 0.0f)
        first_element_pos = pos;

    if (isHovered &&
        IsMouseClicked(0, elements_count, false))
    {
        return true;
    }

    return false;
}


bool ButtonGreen(const char* name, FVector2D size)
{
    elements_count++;

    const float paddingX = 10.0f;
    const float paddingY = 10.0f;

    FVector2D pos = FVector2D{
        menu_pos.X + paddingX + offset_x,
        menu_pos.Y + paddingY + offset_y
    };

    if (sameLine)
    {
        pos.X = last_element_pos.X + last_element_size.X + 10.0f;
        pos.Y = last_element_pos.Y;
    }

    bool isHovered = MouseInZone(pos, size);

    FLinearColor bgColor =
        FLinearColor(0.12f, 0.45f, 0.22f, 1.0f);

    FLinearColor borderColor =
        FLinearColor(0.15f, 0.55f, 0.28f, 1.0f);

    FLinearColor textColor =
        FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);

    if (isHovered)
    {
        bgColor =
            FLinearColor(0.15f, 0.55f, 0.28f, 1.0f);

        borderColor =
            FLinearColor(0.20f, 0.70f, 0.35f, 1.0f);

        hover_element = true;
    }

    DrawRoundRect(
        Canvas,
        pos,
        size.X,
        size.Y,
        4.0f,
        borderColor
    );

    FVector2D innerPos = FVector2D{
        pos.X + 1.0f,
        pos.Y + 1.0f
    };

    FVector2D innerSize = FVector2D{
        size.X - 2.0f,
        size.Y - 2.0f
    };

    DrawRoundRect(
        Canvas,
        innerPos,
        innerSize.X,
        innerSize.Y,
        3.0f,
        bgColor
    );

    FVector2D textPos = FVector2D{
        pos.X + (size.X / 10.0f),
        pos.Y + (size.Y / 2.0f)
    };

    Canvas->K2_DrawText(
        tslFont,
        name,
        textPos,
        textColor,
        1.0f,
        {},
        {},
        false,
        true,
        false,
        {}
    );

    if (!sameLine)
        offset_y += size.Y + paddingY;

    sameLine = false;

    last_element_pos = pos;
    last_element_size = size;

    if (first_element_pos.X == 0.0f)
        first_element_pos = pos;

    if (isHovered &&
        IsMouseClicked(0, elements_count, false))
    {
        return true;
    }

    return false;
}


bool ButtonRed(const char* name, FVector2D size)
{
    elements_count++;

    const float paddingX = 10.0f;
    const float paddingY = 10.0f;

    FVector2D pos = FVector2D{
        menu_pos.X + paddingX + offset_x,
        menu_pos.Y + paddingY + offset_y
    };

    if (sameLine)
    {
        pos.X = last_element_pos.X + last_element_size.X + 10.0f;
        pos.Y = last_element_pos.Y;
    }

    bool isHovered = MouseInZone(pos, size);

    FLinearColor bgColor =
        FLinearColor(0.55f, 0.12f, 0.12f, 1.0f);

    FLinearColor borderColor =
        FLinearColor(0.65f, 0.15f, 0.15f, 1.0f);

    FLinearColor textColor =
        FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);

    if (isHovered)
    {
        bgColor =
            FLinearColor(0.65f, 0.15f, 0.15f, 1.0f);

        borderColor =
            FLinearColor(0.80f, 0.20f, 0.20f, 1.0f);

        hover_element = true;
    }

    DrawRoundRect(
        Canvas,
        pos,
        size.X,
        size.Y,
        4.0f,
        borderColor
    );

    FVector2D innerPos = FVector2D{
        pos.X + 1.0f,
        pos.Y + 1.0f
    };

    FVector2D innerSize = FVector2D{
        size.X - 2.0f,
        size.Y - 2.0f
    };

    DrawRoundRect(
        Canvas,
        innerPos,
        innerSize.X,
        innerSize.Y,
        3.0f,
        bgColor
    );

    FVector2D textPos = FVector2D{
        pos.X + (size.X / 10.0f),
        pos.Y + (size.Y / 2.0f)
    };

    Canvas->K2_DrawText(
        tslFont,
        name,
        textPos,
        textColor,
        1.0f,
        {},
        {},
        false,
        true,
        false,
        {}
    );

    if (!sameLine)
        offset_y += size.Y + paddingY;

    sameLine = false;

    last_element_pos = pos;
    last_element_size = size;

    if (first_element_pos.X == 0.0f)
        first_element_pos = pos;

    if (isHovered &&
        IsMouseClicked(0, elements_count, false))
    {
        return true;
    }

    return false;
}

	
	bool InputBox(const char* name, FVector2D size)
	{
		elements_count++;
		
		FVector2D padding = FVector2D{ 20, 40 };
		FVector2D pos = FVector2D{ menu_pos.X + padding.X + offset_x, menu_pos.Y + padding.Y + offset_y };
		if (sameLine)
		{
			pos.X = last_element_pos.X + last_element_size.X + padding.X + 5.0f;
			pos.Y = last_element_pos.Y;
		}
		if (pushY)
		{
			pos.Y = pushYvalue;
			pushY = false;{}
			pushYvalue = 0.0f;
			offset_y = pos.Y - menu_pos.Y;
		}
		bool isHovered = MouseInZone(FVector2D{ pos.X, pos.Y }, size);
		
        DrawRectangle(Canvas, FVector2D{pos.X, pos.Y}, size.X, size.Y, 1.0f, Colors::BORDER);

		if (!sameLine)
			offset_y += size.Y + padding.Y;

		//Text
		FVector2D textPos = FVector2D{ pos.X + size.X / 2.6, pos.Y + size.Y / 2 };
		TextLeft(name, textPos, FLinearColor{ COLOR_WHITE }, false);

		sameLine = false;
		last_element_pos = pos;
		last_element_size = size;
		if (first_element_pos.X == 0.0f)
			first_element_pos = pos;
			
		last_element_pos = pos;
		last_element_size = FVector2D{ 85, 10 };

		if (isHovered && IsMouseClicked(0, elements_count, false))
			return true;

		return false;
	}
	
bool ButtonLogin(const char* name, FVector2D size)
{
    elements_count++;
    
    FVector2D padding = FVector2D{ 20, 10 };
    FVector2D pos = FVector2D{ menu_pos.X + padding.X + offset_x, menu_pos.Y + padding.Y + offset_y };
    if (sameLine)
    {
        pos.X = last_element_pos.X + last_element_size.X + padding.X + 5.0f;
        pos.Y = last_element_pos.Y;
    }
    if (pushY)
    {
        pos.Y = pushYvalue;
        pushY = false;
        pushYvalue = 0.0f;
        offset_y = pos.Y - menu_pos.Y;
    }
    
    bool isHovered = MouseInZone(FVector2D{ pos.X, pos.Y }, size);
    // التحقق من الضغط
    bool isClicked = isHovered && IsMouseClicked(0, elements_count, false);

    // --- رسم الخلفية بناءً على الحالة ---
    if (isClicked)
    {
        // اللون الأغمق عند الضغط
        drawFilledRect(FVector2D{ pos.X, pos.Y }, size.X, size.Y, Colors::Tab_Active); 
    }
    else if (isHovered)
    {
        // لون عند مرور الماوس (Hover)
        drawFilledRect(FVector2D{ pos.X, pos.Y }, size.X, size.Y, Colors::Tab_Hovered);
        hover_element = true;
    }
    else
    {
        // اللون العادي والماوس بعيد
        drawFilledRect(FVector2D{ pos.X, pos.Y }, size.X, size.Y, Colors::Tab_Idle);
    }
    
    DrawRectangle(Canvas, FVector2D{pos.X, pos.Y}, size.X, size.Y, 1.0f, Colors::BORDER);

    if (!sameLine)
        offset_y += size.Y + padding.Y;

    //Text
    FVector2D textPos = FVector2D{ pos.X + size.X / 2.4, pos.Y + size.Y / 2 };
    TextLeft(name, textPos, Colors::Text2, false);

    sameLine = false;
    last_element_pos = pos;
    last_element_size = size;
    if (first_element_pos.X == 0.0f)
        first_element_pos = pos;
        
    last_element_pos = pos;
    last_element_size = FVector2D{ 85, 10 };

    // إرجاع true لو تم الضغط فعلياً
    if (isClicked)
        return true;

    return false;
}


void DrawFixedCheckboxesBackground(FVector2D menu_pos, float offset_x, float offset_y)
{
    // 1. تحديد الأبعاد الثابتة المطلوبة تماماً
    FVector2D bg_size = FVector2D{ 720.0f, 385.0f };

    // 2. حساب الموضع: يبدأ بعد التابات مباشرة مع مسافة رأسية صغيرة جداً (5 بكسل)
    float small_margin_y = 20.0f;
    FVector2D bg_pos = FVector2D{ menu_pos.X + offset_x + 10.0f, menu_pos.Y + offset_y + small_margin_y };

    // 3. الألوان (رصاصي فيراني داكن وحدود سوداء صريحة)
    FLinearColor charcoal_gray = FLinearColor{0.06f, 0.06f, 0.06f, 8.0f }; 
    //0.08f, 0.09f, 0.10f, 1.0f
    FLinearColor pure_black    = FLinearColor{ 0.00f, 0.00f, 0.00f, 1.0f }; 

    // 4. رسم الصندوق والحدود
    drawFilledRect(bg_pos, bg_size.X, bg_size.Y, charcoal_gray);
    DrawRectangle(Canvas, bg_pos, bg_size.X, bg_size.Y, 1.5f, pure_black);
}



	
	
// ── دالة زوايا ناعمة حقيقية ──
void DrawRoundedFilledRect(FVector2D pos, float w, float h, float r, FLinearColor color)
{
    // Optimized pill shape.
    // A pill only needs two end circles instead of four corner circles.
    if (w <= 0.0f || h <= 0.0f)
        return;

    r = fminf(r, h * 0.5f);
    r = fminf(r, w * 0.5f);

    drawFilledRect(
        {pos.X + r, pos.Y},
        w - (r * 2.0f),
        h,
        color
    );

    DrawFilledCircle(
        {pos.X + r, pos.Y + r},
        r,
        color
    );

    DrawFilledCircle(
        {pos.X + w - r, pos.Y + r},
        r,
        color
    );
}




bool Checkbox(const char* name, bool* value)
{
    elements_count++;

    const FVector2D size = FVector2D{ 45.f, 24.f };
    const FVector2D padding = FVector2D{ 10.f, 10.f };

    FVector2D pos = FVector2D{
        menu_pos.X + padding.X + offset_x,
        menu_pos.Y + padding.Y + offset_y
    };

    if (sameLine)
    {
        pos.X = last_element_pos.X + last_element_size.X + 55.f;
        pos.Y = last_element_pos.Y;
    }

    // =========================
    // Animation
    // =========================

    static std::map<bool*, float> animValues;

    if (animValues.find(value) == animValues.end())
        animValues[value] = *value ? 1.0f : 0.0f;

    float target = *value ? 1.0f : 0.0f;

    animValues[value] +=
        (target - animValues[value]) * 0.18f;

    float animation = animValues[value];

    // =========================
    // Colors
    // =========================

    // OFF = رصاصي
    // OFF = رصاصي غامق
FLinearColor offColor =
    FLinearColor{ 0.20f, 0.20f, 0.20f, 1.0f };

    // ON = بنفسجي
    FLinearColor onColor =
        FLinearColor{ 1.0f, 0.0f, 0.0f, 1.0f };

    // المقبض نفس اللون في الحالتين
    FLinearColor handleColor =
        FLinearColor{ 0.45f, 0.45f, 0.45f, 1.0f };

    // =========================
    // Hover
    // =========================

    FVector2D hitZoneSize =
        FVector2D{ size.X + 110.f, size.Y };

    bool isHovered =
        MouseInZone(pos, hitZoneSize);

    // =========================
    // Switch Color
    // =========================

    FLinearColor switchColor =
        FLinearColor{
            offColor.R +
                (onColor.R - offColor.R) * animation,

            offColor.G +
                (onColor.G - offColor.G) * animation,

            offColor.B +
                (onColor.B - offColor.B) * animation,

            1.0f
        };

    if (isHovered)
    {
        switchColor.R += 0.02f;
        switchColor.G += 0.02f;
        switchColor.B += 0.02f;
    }

    // =========================
    // Switch
    // =========================

    DrawRoundRect(
        Canvas,
        pos,
        size.X,
        size.Y,
        4.0f,
        switchColor
    );

    // =========================
    // Handle
    // =========================

    const float handleSize = 16.0f;
    const float handlePadding = 4.0f;

    float minHandleX =
        pos.X + handlePadding;

    float maxHandleX =
        pos.X +
        size.X -
        handleSize -
        handlePadding;

    float handleX =
        minHandleX +
        (maxHandleX - minHandleX) *
        animation;

    FVector2D handlePos = FVector2D{
        handleX,
        pos.Y + (size.Y - handleSize) / 2.0f
    };

    // المقبض ثابت اللون
    DrawRoundRect(
        Canvas,
        handlePos,
        handleSize,
        handleSize,
        3.0f,
        handleColor
    );

    // =========================
    // Text Color
    // =========================

    FLinearColor textColor;

    if (*value)
    {
        // ON = أبيض
        textColor =
            FLinearColor{ 1.00f, 1.00f, 1.00f, 1.0f };
    }
    else
    {
        // OFF = رصاصي
        textColor =
            FLinearColor{ 0.55f, 0.55f, 0.55f, 1.0f };
    }

    // =========================
    // Text
    // =========================

    FVector2D textPos = FVector2D{
        pos.X + size.X + 14.f,
        pos.Y + size.Y / 2.0f
    };

    Canvas->K2_DrawText(
        tslFont,
        name,
        textPos,
        textColor,
        1.0f,
        {},
        {},
        false,
        true,
        false,
        {}
    );

    // =========================
    // Position
    // =========================

    if (!sameLine)
        offset_y += size.Y + 14.f;

    sameLine = false;

    last_element_pos = pos;
    last_element_size = hitZoneSize;

    // =========================
    // Click
    // =========================

    if (isHovered &&
        IsMouseClicked(0, elements_count, false))
    {
        *value = !*value;
        return true;
    }

    return false;
}




	bool CheckCircle(char* name, bool* value)
	{
		elements_count++;
		
		float size = 30;
		FVector2D padding = FVector2D{ 10, 10 };
		FVector2D pos = FVector2D{ menu_pos.X + padding.X + offset_x, menu_pos.Y + padding.Y + offset_y };
		if (sameLine)
		{
			pos.X = last_element_pos.X + last_element_size.X + padding.X;
			pos.Y = last_element_pos.Y;
		}
		if (pushY)
		{
			pos.Y = pushYvalue;
			pushY = false;
			pushYvalue = 0.0f;
			offset_y = pos.Y - menu_pos.Y;
		}
		bool isHovered = MouseInZone(FVector2D{ pos.X, pos.Y }, FVector2D{ size, size });
		
		//Bg
		if (isHovered)
		{
			hover_element = true;
		}
		
		DrawCircle(FVector2D{ pos.X+15.0f, pos.Y+15.0f }, 15.0f, 15.0f, Colors::Checkbox_Idle);
		// drawFilledRect(FVector2D{ pos.X, pos.Y }, size, size, Colors::Checkbox_Idle);
	
		if (!sameLine)
			offset_y += size + padding.Y;
	
		if (*value)
		{
		    DrawFilledCircle(FVector2D{ pos.X+15.0f, pos.Y+15.0f}, 12.0f, Colors::Checkbox_Enabled);
		}
								
		//Text
		FVector2D textPos = FVector2D{ pos.X + size + 5.0f, pos.Y + size / 2 };
		//if (!TextOverlapedFromActiveElement(textPos))
			TextLeft(name, textPos, FLinearColor{1.0f, 1.0f, 1.0f, 1.0f}, false);
		
		
		sameLine = false;
		last_element_pos = pos;
		//last_element_size = size;
		if (first_element_pos.X == 0.0f)
			first_element_pos = pos;
		
		if (isHovered && IsMouseClicked(0, elements_count, false)){
			*value = !*value;
			return true;
	    }
	    return false;
	}
	
	
bool SliderFloat(const char* name, float* value, float min, float max, const char* format = "%.0f")
{
    elements_count++;

    FVector2D size = FVector2D{ 420.f, 40.f };
    FVector2D slider_size = size;

    // المسافة من حافة المنيو = 10px
    const float menuPaddingX = 10.0f;
    const float menuPaddingY = 10.0f;

    FVector2D pos = FVector2D{
        menu_pos.X + menuPaddingX + offset_x,
        menu_pos.Y + menuPaddingY + offset_y
    };

    if (sameLine)
    {
        pos.X = last_element_pos.X + last_element_size.X + 10.0f;
        pos.Y = last_element_pos.Y;
    }

    if (pushY)
    {
        pos.Y = pushYvalue;
        pushY = false;
        pushYvalue = 0.0f;
        offset_y = pos.Y - menu_pos.Y;
    }

    bool isHovered = MouseInZone(pos, size);

    if (!sameLine)
        offset_y += size.Y + menuPaddingY;

    bool change = false;

    if (isHovered || current_element == elements_count)
    {
        if (IsMouseClicked(0, elements_count, true))
        {
            current_element = elements_count;

            FVector2D cursorPos = CursorPos();

            *value =
                ((cursorPos.X - pos.X) *
                ((max - min) / slider_size.X)) + min;

            if (*value < min)
                *value = min;

            if (*value > max)
                *value = max;

            change = true;
        }

        hover_element = true;
    }

    // =========================
    // Colors
    // =========================

    FLinearColor bgColor =
        FLinearColor{ 0.14f, 0.14f, 0.14f, 1.0f };

    FLinearColor borderColor =
        FLinearColor{ 0.24f, 0.24f, 0.24f, 1.0f };

    FLinearColor lineIndicatorColor = FLinearColor{ 1.0f, 0.0f, 0.0f, 1.0f };

    if (isHovered || current_element == elements_count)
    {
        borderColor =
            FLinearColor{ 0.40f, 0.40f, 0.40f, 1.0f };

        bgColor.R += 0.01f;
        bgColor.G += 0.01f;
        bgColor.B += 0.01f;
    }

    // =========================
    // Outer
    // =========================

    DrawRoundRect(
        Canvas,
        pos,
        size.X,
        size.Y,
        4.0f,
        borderColor
    );

    // =========================
    // Inner
    // =========================

    FVector2D innerPos = FVector2D{
        pos.X + 1.f,
        pos.Y + 1.f
    };

    FVector2D innerSize = FVector2D{
        size.X - 2.f,
        size.Y - 2.f
    };

    DrawRoundRect(
        Canvas,
        innerPos,
        innerSize.X,
        innerSize.Y,
        3.0f,
        bgColor
    );

    // =========================
    // Percentage
    // =========================

    float percentage =
        (*value - min) / (max - min);

    // =========================
    // Slider Line
    // =========================

    float linePaddingX = 4.0f;

    float minLineX =
        innerPos.X + linePaddingX;

    float maxLineX =
        innerPos.X + innerSize.X - linePaddingX;

    float currentLineX =
        minLineX +
        (maxLineX - minLineX) * percentage;

    float lineWidth = 3.0f;
    float linePaddingY = 3.0f;

    FVector2D linePos = FVector2D{
        currentLineX - (lineWidth / 2.0f),
        innerPos.Y + linePaddingY
    };

    FVector2D lineSize = FVector2D{
        lineWidth,
        innerSize.Y - (linePaddingY * 2.0f)
    };

    DrawRoundRect(
        Canvas,
        linePos,
        lineSize.X,
        lineSize.Y,
        1.0f,
        lineIndicatorColor
    );

    // =========================
    // Name
    // =========================

    FVector2D textPos = FVector2D{
        pos.X + 8.0f,
        pos.Y +
        slider_size.Y / 2.0f +
        menuPaddingY - 5.0f
    };

    TextLeft(
        name,
        textPos,
        Colors::Text2,
        false
    );

    // =========================
    // Value
    // =========================

    char buffer[64];

    snprintf(
        buffer,
        sizeof(buffer),
        "%.0f / %.0f",
        *value,
        max
    );

    FVector2D valuePos = FVector2D{
        pos.X + slider_size.X - 65.0f,
        pos.Y +
        slider_size.Y / 2.0f +
        menuPaddingY - 5.0f
    };

    TextCenter(
        buffer,
        valuePos,
        Colors::Text2,
        false
    );

    // =========================
    // Position
    // =========================

    sameLine = false;

    last_element_pos = pos;
    last_element_size = size;

    if (first_element_pos.X == 0.0f)
        first_element_pos = pos;

    return change;
}
    
bool checkbox_enabled[256]; // 用于存储复选框的启用状态的数组

// 渲染下拉框 UI 元素的函数
bool Combobox(char* name, FVector2D size, int* value, const char* arg, ...)
{
    elements_count++; // 增加 UI 元素的计数

    FVector2D padding = FVector2D{ 5, 10 };
    FVector2D pos = FVector2D{ menu_pos.X + padding.X + offset_x, menu_pos.Y + padding.Y + offset_y };
    if (sameLine) // 如果元素应该在与上一个元素相同的行上渲染
    {
        pos.X = last_element_pos.X + last_element_size.X + padding.X;
        pos.Y = last_element_pos.Y;
    }
    if (pushY) // 如果需要推动 Y 轴位置
    {
        pos.Y = pushYvalue;
        pushY = false;
        pushYvalue = 0.0f;
        offset_y = pos.Y - menu_pos.Y;
    }
    bool isHovered = MouseInZone(FVector2D{ pos.X, pos.Y }, size);

    // 背景
    if (isHovered || checkbox_enabled[elements_count])
    {
        drawFilledRect(FVector2D{ pos.X, pos.Y }, size.X, size.Y, Colors::Combobox_Hovered);
        hover_element = true;
    }
    else
    {
        drawFilledRect(FVector2D{ pos.X, pos.Y }, size.X, size.Y, Colors::Combobox_Idle);
    }

    if (!sameLine)
        offset_y += size.Y + padding.Y;

    // 文本
    FVector2D textPos = FVector2D{ pos.X + size.X + 5.0f, pos.Y + size.Y / 2 };
    TextLeft(name, textPos, FLinearColor{ 1.0f, 1.0f, 1.0f, 1.0f }, false);

    // 元素
    bool isHovered2 = false;
    FVector2D element_pos = pos;
    int num = 0;

    if (checkbox_enabled[elements_count])
    {
        current_element_size.X = element_pos.X - 5.0f;
        current_element_size.Y = element_pos.Y - 5.0f;
    }
    va_list arguments;
    for (va_start(arguments, arg); arg != NULL; arg = va_arg(arguments, const char*))
    {
        // 选中的元素
        if (num == *value)
        {
            FVector2D _textPos = FVector2D{ pos.X + size.X / 2, pos.Y + size.Y / 2 };
            TextCenter((char*)arg, _textPos, FLinearColor{ 1.0f, 1.0f, 1.0f, 1.0f }, false);
        }

        if (checkbox_enabled[elements_count])
        {
            element_pos.Y += 25.0f;

            isHovered2 = MouseInZone(FVector2D{ element_pos.X, element_pos.Y }, FVector2D{ size.X, 25.0f });
            if (isHovered2)
            {
                hover_element = true;
                PostRenderer::drawFilledRect(FVector2D{ element_pos.X, element_pos.Y }, size.X, 25.0f, Colors::Combobox_Hovered);

                // 单击事件
                if (IsMouseClicked(0, elements_count, false))
                {
                    *value = num;
                    checkbox_enabled[elements_count] = false;
                }
            }
            else
            {
                PostRenderer::drawFilledRect(FVector2D{ element_pos.X, element_pos.Y }, size.X, 25.0f, Colors::Combobox_Idle);
            }

            PostRenderer::TextLeft((char*)arg, FVector2D{ element_pos.X + 5.0f, element_pos.Y + 15.0f }, FLinearColor{ 1.0f, 1.0f, 1.0f, 1.0f }, false);
        }
        num++;
    }
    va_end(arguments);
    if (checkbox_enabled[elements_count])
    {
        current_element_size.X = element_pos.X + 5.0f;
        current_element_size.Y = element_pos.Y + 5.0f;
    }

    sameLine = false;
    last_element_pos = pos;
    last_element_size = size;
    if (first_element_pos.X == 0.0f)
        first_element_pos = pos;

    if (isHovered && IsMouseClicked(0, elements_count, false)){
        *value = !*value;
        return true;
    }
    return false;
}

	int active_picker = -1;
	FLinearColor saved_color;
	bool ColorPixel(FVector2D pos, FVector2D size, FLinearColor* original, FLinearColor color)
	{
		PostRenderer::drawFilledRect(FVector2D{ pos.X, pos.Y }, size.X, size.Y, color);

		//Выбранный цвет
		if (original->R == color.R && original->G == color.G && original->B == color.B)
		{
			PostRenderer::Draw_Line(FVector2D{ pos.X, pos.Y }, FVector2D{ pos.X + size.X - 1, pos.Y }, 1.0f, FLinearColor{ 0.0f, 0.0f, 0.0f, 1.0f });
			PostRenderer::Draw_Line(FVector2D{ pos.X, pos.Y + size.Y - 1 }, FVector2D{ pos.X + size.X - 1, pos.Y + size.Y - 1 }, 1.0f, FLinearColor{ 0.0f, 0.0f, 0.0f, 1.0f });
			PostRenderer::Draw_Line(FVector2D{ pos.X, pos.Y }, FVector2D{ pos.X, pos.Y + size.Y - 1 }, 1.0f, FLinearColor{ 0.0f, 0.0f, 0.0f, 1.0f });
			PostRenderer::Draw_Line(FVector2D{ pos.X + size.X - 1, pos.Y }, FVector2D{ pos.X + size.X - 1, pos.Y + size.Y - 1 }, 1.0f, FLinearColor{ 0.0f, 0.0f, 0.0f, 1.0f });
		}

		//Смена цвета
		bool isHovered = MouseInZone(FVector2D{ pos.X, pos.Y }, size);
		if (isHovered)
		{
			if (IsMouseClicked(0, elements_count, false))
				*original = color;
		}

		return true;
	}
	void ColorPicker(const char* name, FLinearColor* color)
	{
		elements_count++;

		float size = 25;
		FVector2D padding = FVector2D{ 10, 10 };
		FVector2D pos = FVector2D{ menu_pos.X + padding.X + offset_x, menu_pos.Y + padding.Y + offset_y };
		if (sameLine)
		{
			pos.X = last_element_pos.X + last_element_size.X + padding.X;
			pos.Y = last_element_pos.Y;
		}
		if (pushY)
		{
			pos.Y = pushYvalue;
			pushY = false;
			pushYvalue = 0.0f;
			offset_y = pos.Y - menu_pos.Y;
		}
		bool isHovered = MouseInZone(FVector2D{ pos.X, pos.Y }, FVector2D{ size, size });

		if (!sameLine)
			offset_y += size + padding.Y;

		if (active_picker == elements_count)
		{
			hover_element = true;

			float sizePickerX = 250;
			float sizePickerY = 250;
			bool isHoveredPicker = MouseInZone(FVector2D{ pos.X, pos.Y }, FVector2D{ sizePickerX, sizePickerY - 60 });

			//Background
			PostRenderer::drawFilledRect(FVector2D{ pos.X, pos.Y }, sizePickerX, sizePickerY - 65, Colors::ColorPicker_Background);

			FVector2D pixelSize = FVector2D{ sizePickerX/12, sizePickerY/12 };

			//0
			{
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 0, pos.Y + pixelSize.Y * 0 }, pixelSize, color, FLinearColor{ 174/255.f, 235/255.f, 253/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 0, pos.Y + pixelSize.Y * 1 }, pixelSize, color, FLinearColor{ 136/255.f, 225/255.f, 251/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 0, pos.Y + pixelSize.Y * 2 }, pixelSize, color, FLinearColor{ 108/255.f, 213/255.f, 250/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 0, pos.Y + pixelSize.Y * 3 }, pixelSize, color, FLinearColor{ 89/255.f, 175/255.f, 213/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 0, pos.Y + pixelSize.Y * 4 }, pixelSize, color, FLinearColor{ 76/255.f, 151/255.f, 177/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 0, pos.Y + pixelSize.Y * 5 }, pixelSize, color, FLinearColor{ 60/255.f, 118/255.f, 140/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 0, pos.Y + pixelSize.Y * 6 }, pixelSize, color, FLinearColor{ 43/255.f, 85/255.f, 100/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 0, pos.Y + pixelSize.Y * 7 }, pixelSize, color, FLinearColor{ 32/255.f, 62/255.f, 74/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 0, pos.Y + pixelSize.Y * 8 }, pixelSize, color, FLinearColor{ 255/255.f, 255/255.f, 255/255.f, 1.0f });
			}
			//1
			{
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 1, pos.Y + pixelSize.Y * 0 }, pixelSize, color, FLinearColor{ 175/255.f, 205/255.f, 252/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 1, pos.Y + pixelSize.Y * 1 }, pixelSize, color, FLinearColor{ 132/255.f, 179/255.f, 252/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 1, pos.Y + pixelSize.Y * 2 }, pixelSize, color, FLinearColor{ 90/255.f, 152/255.f, 250/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 1, pos.Y + pixelSize.Y * 3 }, pixelSize, color, FLinearColor{ 55/255.f, 120/255.f, 250/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 1, pos.Y + pixelSize.Y * 4 }, pixelSize, color, FLinearColor{ 49/255.f, 105/255.f, 209/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 1, pos.Y + pixelSize.Y * 5 }, pixelSize, color, FLinearColor{ 38/255.f, 83/255.f, 165/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 1, pos.Y + pixelSize.Y * 6 }, pixelSize, color, FLinearColor{ 28/255.f, 61/255.f, 120/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 1, pos.Y + pixelSize.Y * 7 }, pixelSize, color, FLinearColor{ 20/255.f, 43/255.f, 86/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 1, pos.Y + pixelSize.Y * 8 }, pixelSize, color, FLinearColor{ 247/255.f, 247/255.f, 247/255.f, 1.0f });
			}
			//2
			{
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 2, pos.Y + pixelSize.Y * 0 }, pixelSize, color, FLinearColor{ 153/255.f, 139/255.f, 250/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 2, pos.Y + pixelSize.Y * 1 }, pixelSize, color, FLinearColor{ 101/255.f, 79/255.f, 249/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 2, pos.Y + pixelSize.Y * 2 }, pixelSize, color, FLinearColor{ 64/255.f, 50/255.f, 230/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 2, pos.Y + pixelSize.Y * 3 }, pixelSize, color, FLinearColor{ 54/255.f, 38/255.f, 175/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 2, pos.Y + pixelSize.Y * 4 }, pixelSize, color, FLinearColor{ 39/255.f, 31/255.f, 144/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 2, pos.Y + pixelSize.Y * 5 }, pixelSize, color, FLinearColor{ 32/255.f, 25/255.f, 116/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 2, pos.Y + pixelSize.Y * 6 }, pixelSize, color, FLinearColor{ 21/255.f, 18/255.f, 82/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 2, pos.Y + pixelSize.Y * 7 }, pixelSize, color, FLinearColor{ 16/255.f, 13/255.f, 61/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 2, pos.Y + pixelSize.Y * 8 }, pixelSize, color, FLinearColor{ 228/255.f, 228/255.f, 228/255.f, 1.0f });
			}
			//3
			{
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 3, pos.Y + pixelSize.Y * 0 }, pixelSize, color, FLinearColor{ 194/255.f, 144/255.f, 251/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 3, pos.Y + pixelSize.Y * 1 }, pixelSize, color, FLinearColor{ 165/255.f, 87/255.f, 249/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 3, pos.Y + pixelSize.Y * 2 }, pixelSize, color, FLinearColor{ 142/255.f, 57/255.f, 239/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 3, pos.Y + pixelSize.Y * 3 }, pixelSize, color, FLinearColor{ 116/255.f, 45/255.f, 184/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 3, pos.Y + pixelSize.Y * 4 }, pixelSize, color, FLinearColor{ 92/255.f, 37/255.f, 154/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 3, pos.Y + pixelSize.Y * 5 }, pixelSize, color, FLinearColor{ 73/255.f, 29/255.f, 121/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 3, pos.Y + pixelSize.Y * 6 }, pixelSize, color, FLinearColor{ 53/255.f, 21/255.f, 88/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 3, pos.Y + pixelSize.Y * 7 }, pixelSize, color, FLinearColor{ 37/255.f, 15/255.f, 63/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 3, pos.Y + pixelSize.Y * 8 }, pixelSize, color, FLinearColor{ 203/255.f, 203/255.f, 203/255.f, 1.0f });
			}
			//4
			{
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 4, pos.Y + pixelSize.Y * 0 }, pixelSize, color, FLinearColor{ 224/255.f, 162/255.f, 197/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 4, pos.Y + pixelSize.Y * 1 }, pixelSize, color, FLinearColor{ 210/255.f, 112/255.f, 166/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 4, pos.Y + pixelSize.Y * 2 }, pixelSize, color, FLinearColor{ 199/255.f, 62/255.f, 135/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 4, pos.Y + pixelSize.Y * 3 }, pixelSize, color, FLinearColor{ 159/255.f, 49/255.f, 105/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 4, pos.Y + pixelSize.Y * 4 }, pixelSize, color, FLinearColor{ 132/255.f, 41/255.f, 89/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 4, pos.Y + pixelSize.Y * 5 }, pixelSize, color, FLinearColor{ 104/255.f, 32/255.f, 71/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 4, pos.Y + pixelSize.Y * 6 }, pixelSize, color, FLinearColor{ 75/255.f, 24/255.f, 51/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 4, pos.Y + pixelSize.Y * 7 }, pixelSize, color, FLinearColor{ 54/255.f, 14/255.f, 36/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 4, pos.Y + pixelSize.Y * 8 }, pixelSize, color, FLinearColor{ 175/255.f, 175/255.f, 175/255.f, 1.0f });
			}
			//5
			{
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 5, pos.Y + pixelSize.Y * 0 }, pixelSize, color, FLinearColor{ 235/255.f, 175/255.f, 176/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 5, pos.Y + pixelSize.Y * 1 }, pixelSize, color, FLinearColor{ 227/255.f, 133/255.f, 135/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 5, pos.Y + pixelSize.Y * 2 }, pixelSize, color, FLinearColor{ 219/255.f, 87/255.f, 88/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 5, pos.Y + pixelSize.Y * 3 }, pixelSize, color, FLinearColor{ 215/255.f, 50/255.f, 36/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 5, pos.Y + pixelSize.Y * 4 }, pixelSize, color, FLinearColor{ 187/255.f, 25/255.f, 7/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 5, pos.Y + pixelSize.Y * 5 }, pixelSize, color, FLinearColor{ 149/255.f, 20/255.f, 6/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 5, pos.Y + pixelSize.Y * 6 }, pixelSize, color, FLinearColor{ 107/255.f, 14/255.f, 4/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 5, pos.Y + pixelSize.Y * 7 }, pixelSize, color, FLinearColor{ 77/255.f, 9/255.f, 3/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 5, pos.Y + pixelSize.Y * 8 }, pixelSize, color, FLinearColor{ 144/255.f, 144/255.f, 144/255.f, 1.0f });
			}
			//6
			{
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 6, pos.Y + pixelSize.Y * 0 }, pixelSize, color, FLinearColor{ 241/255.f, 187/255.f, 171/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 6, pos.Y + pixelSize.Y * 1 }, pixelSize, color, FLinearColor{ 234/255.f, 151/255.f, 126/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 6, pos.Y + pixelSize.Y * 2 }, pixelSize, color, FLinearColor{ 229/255.f, 115/255.f, 76/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 6, pos.Y + pixelSize.Y * 3 }, pixelSize, color, FLinearColor{ 227/255.f, 82/255.f, 24/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 6, pos.Y + pixelSize.Y * 4 }, pixelSize, color, FLinearColor{ 190/255.f, 61/255.f, 15/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 6, pos.Y + pixelSize.Y * 5 }, pixelSize, color, FLinearColor{ 150/255.f, 48/255.f, 12/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 6, pos.Y + pixelSize.Y * 6 }, pixelSize, color, FLinearColor{ 107/255.f, 34/255.f, 8/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 6, pos.Y + pixelSize.Y * 7 }, pixelSize, color, FLinearColor{ 79/255.f, 25/255.f, 6/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 6, pos.Y + pixelSize.Y * 8 }, pixelSize, color, FLinearColor{ 113/255.f, 113/255.f, 113/255.f, 1.0f });
			}
			//7
			{
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 7, pos.Y + pixelSize.Y * 0 }, pixelSize, color, FLinearColor{ 245/255.f, 207/255.f, 169/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 7, pos.Y + pixelSize.Y * 1 }, pixelSize, color, FLinearColor{ 240/255.f, 183/255.f, 122/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 7, pos.Y + pixelSize.Y * 2 }, pixelSize, color, FLinearColor{ 236/255.f, 159/255.f, 74/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 7, pos.Y + pixelSize.Y * 3 }, pixelSize, color, FLinearColor{ 234/255.f, 146/255.f, 37/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 7, pos.Y + pixelSize.Y * 4 }, pixelSize, color, FLinearColor{ 193/255.f, 111/255.f, 28/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 7, pos.Y + pixelSize.Y * 5 }, pixelSize, color, FLinearColor{ 152/255.f, 89/255.f, 22/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 7, pos.Y + pixelSize.Y * 6 }, pixelSize, color, FLinearColor{ 110/255.f, 64/255.f, 16/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 7, pos.Y + pixelSize.Y * 7 }, pixelSize, color, FLinearColor{ 80/255.f, 47/255.f, 12/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 7, pos.Y + pixelSize.Y * 8 }, pixelSize, color, FLinearColor{ 82/255.f, 82/255.f, 82/255.f, 1.0f });
			}
			//8
			{
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 8, pos.Y + pixelSize.Y * 0 }, pixelSize, color, FLinearColor{ 247/255.f, 218/255.f, 170/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 8, pos.Y + pixelSize.Y * 1 }, pixelSize, color, FLinearColor{ 244/255.f, 200/255.f, 124/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 8, pos.Y + pixelSize.Y * 2 }, pixelSize, color, FLinearColor{ 241/255.f, 182/255.f, 77/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 8, pos.Y + pixelSize.Y * 3 }, pixelSize, color, FLinearColor{ 239/255.f, 174/255.f, 44/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 8, pos.Y + pixelSize.Y * 4 }, pixelSize, color, FLinearColor{ 196/255.f, 137/255.f, 34/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 8, pos.Y + pixelSize.Y * 5 }, pixelSize, color, FLinearColor{ 154/255.f, 108/255.f, 27/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 8, pos.Y + pixelSize.Y * 6 }, pixelSize, color, FLinearColor{ 111/255.f, 77/255.f, 19/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 8, pos.Y + pixelSize.Y * 7 }, pixelSize, color, FLinearColor{ 80/255.f, 56/255.f, 14/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 8, pos.Y + pixelSize.Y * 8 }, pixelSize, color, FLinearColor{ 54/255.f, 54/255.f, 54/255.f, 1.0f });
			}
			//9
			{
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 9, pos.Y + pixelSize.Y * 0 }, pixelSize, color, FLinearColor{ 254/255.f, 243/255.f, 187/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 9, pos.Y + pixelSize.Y * 1 }, pixelSize, color, FLinearColor{ 253/255.f, 237/255.f, 153/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 9, pos.Y + pixelSize.Y * 2 }, pixelSize, color, FLinearColor{ 253/255.f, 231/255.f, 117/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 9, pos.Y + pixelSize.Y * 3 }, pixelSize, color, FLinearColor{ 254/255.f, 232/255.f, 85/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 9, pos.Y + pixelSize.Y * 4 }, pixelSize, color, FLinearColor{ 242/255.f, 212/255.f, 53/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 9, pos.Y + pixelSize.Y * 5 }, pixelSize, color, FLinearColor{ 192/255.f, 169/255.f, 42/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 9, pos.Y + pixelSize.Y * 6 }, pixelSize, color, FLinearColor{ 138/255.f, 120/255.f, 30/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 9, pos.Y + pixelSize.Y * 7 }, pixelSize, color, FLinearColor{ 101/255.f, 87/255.f, 22/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 9, pos.Y + pixelSize.Y * 8 }, pixelSize, color, FLinearColor{ 29/255.f, 29/255.f, 29/255.f, 1.0f });
			}
			//10
			{
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 10, pos.Y + pixelSize.Y * 0 }, pixelSize, color, FLinearColor{ 247/255.f, 243/255.f, 185/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 10, pos.Y + pixelSize.Y * 1 }, pixelSize, color, FLinearColor{ 243/255.f, 239/255.f, 148/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 10, pos.Y + pixelSize.Y * 2 }, pixelSize, color, FLinearColor{ 239/255.f, 232/255.f, 111/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 10, pos.Y + pixelSize.Y * 3 }, pixelSize, color, FLinearColor{ 235/255.f, 229/255.f, 76/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 10, pos.Y + pixelSize.Y * 4 }, pixelSize, color, FLinearColor{ 208/255.f, 200/255.f, 55/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 10, pos.Y + pixelSize.Y * 5 }, pixelSize, color, FLinearColor{ 164/255.f, 157/255.f, 43/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 10, pos.Y + pixelSize.Y * 6 }, pixelSize, color, FLinearColor{ 118/255.f, 114/255.f, 31/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 10, pos.Y + pixelSize.Y * 7 }, pixelSize, color, FLinearColor{ 86/255.f, 82/255.f, 21/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 10, pos.Y + pixelSize.Y * 8 }, pixelSize, color, FLinearColor{ 9/255.f, 9/255.f, 9/255.f, 1.0f });
			}
			//11
			{
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 11, pos.Y + pixelSize.Y * 0 }, pixelSize, color, FLinearColor{ 218/255.f, 232/255.f, 182/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 11, pos.Y + pixelSize.Y * 1 }, pixelSize, color, FLinearColor{ 198/255.f, 221/255.f, 143/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 11, pos.Y + pixelSize.Y * 2 }, pixelSize, color, FLinearColor{ 181/255.f, 210/255.f, 103/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 11, pos.Y + pixelSize.Y * 3 }, pixelSize, color, FLinearColor{ 154/255.f, 186/255.f, 76/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 11, pos.Y + pixelSize.Y * 4 }, pixelSize, color, FLinearColor{ 130/255.f, 155/255.f, 64/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 11, pos.Y + pixelSize.Y * 5 }, pixelSize, color, FLinearColor{ 102/255.f, 121/255.f, 50/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 11, pos.Y + pixelSize.Y * 6 }, pixelSize, color, FLinearColor{ 74/255.f, 88/255.f, 36/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 11, pos.Y + pixelSize.Y * 7 }, pixelSize, color, FLinearColor{ 54/255.f, 64/255.f, 26/255.f, 1.0f });
				ColorPixel(FVector2D{ pos.X + pixelSize.X * 11, pos.Y + pixelSize.Y * 8 }, pixelSize, color, FLinearColor{ 0/255.f, 0/255.f, 0/255.f, 1.0f });
			}
						
			if (isHoveredPicker)
			{
				if (IsMouseClicked(0, elements_count, false))
				{

				}
			}
			else
			{
				if (IsMouseClicked(0, elements_count, false))
				{
					active_picker = -1;
					//hover_element = false;
				}
			}
		}
		else
		{
			//Bg
			if (isHovered)
			{
				drawFilledRect(FVector2D{ pos.X, pos.Y }, size, size, Colors::Checkbox_Hovered);
				hover_element = true;
			}
			else
			{
				drawFilledRect(FVector2D{ pos.X, pos.Y }, size, size, Colors::Checkbox_Idle);
			}

			//Color
			drawFilledRect(FVector2D{ pos.X + 4, pos.Y + 4 }, size - 8, size - 8, *color);

			//Text
			FVector2D textPos = FVector2D{ pos.X + size + 5.0f, pos.Y + size / 2 };
			TextLeft(name, textPos, FLinearColor{ 1.0f, 1.0f, 1.0f, 1.0f }, false);

			if (isHovered && IsMouseClicked(0, elements_count, false))
			{
				saved_color = *color;
				active_picker = elements_count;
			}
		}


		sameLine = false;
		last_element_pos = pos;
		//last_element_size = size;
		if (first_element_pos.X == 0.0f)
			first_element_pos = pos;
	}
	
	void onEvent(AInputEvent *input_event, Vector2 screen_scale) {
    auto event_type = AInputEvent_getType(input_event);
    switch (event_type) {
    
        case AINPUT_EVENT_TYPE_KEY: {
            int32_t event_key_code = AKeyEvent_getKeyCode(input_event);
            int32_t event_action = AKeyEvent_getAction(input_event);
            int32_t event_meta_state = AKeyEvent_getMetaState(input_event);

            KeyCtrl = ((event_meta_state & AMETA_CTRL_ON) != 0);
            KeyShift = ((event_meta_state & AMETA_SHIFT_ON) != 0);
            KeyAlt = ((event_meta_state & AMETA_ALT_ON) != 0);

            switch (event_action) {
                case AKEY_EVENT_ACTION_DOWN:
                case AKEY_EVENT_ACTION_UP:
                    g_KeyEventQueues[event_key_code].push(event_action);
                    break;
                default:
                    break;
            }
            break;
        }
        
        case AINPUT_EVENT_TYPE_MOTION: {
            int32_t event_action = AMotionEvent_getAction(input_event);
            int32_t event_pointer_index = (event_action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
            event_action &= AMOTION_EVENT_ACTION_MASK;
            switch (event_action) {
          case AMOTION_EVENT_ACTION_DOWN:
          case AMOTION_EVENT_ACTION_UP:
          if ((AMotionEvent_getToolType(input_event, event_pointer_index) == AMOTION_EVENT_TOOL_TYPE_FINGER) || (AMotionEvent_getToolType(input_event, event_pointer_index) == AMOTION_EVENT_TOOL_TYPE_UNKNOWN)) {
              MouseDown = (event_action == AMOTION_EVENT_ACTION_DOWN);
              FVector2D pos(AMotionEvent_getRawX(input_event, event_pointer_index), AMotionEvent_getRawY(input_event, event_pointer_index));
              MousePos = FVector2D(screen_scale.X > 0 ? pos.X / screen_scale.X : pos.X, screen_scale.Y > 0 ? pos.Y / screen_scale.Y : pos.Y);
          }
            break;
          case AMOTION_EVENT_ACTION_BUTTON_PRESS:
          case AMOTION_EVENT_ACTION_BUTTON_RELEASE: {
                int32_t button_state = AMotionEvent_getButtonState(input_event);
                MouseDown = ((button_state & AMOTION_EVENT_BUTTON_PRIMARY) != 0);

          }
            break;
          case AMOTION_EVENT_ACTION_HOVER_MOVE: // Hovering: Tool moves while NOT pressed (such as a physical mouse)
          case AMOTION_EVENT_ACTION_MOVE: {       // Touch pointer moves while DOWN
                FVector2D pos(AMotionEvent_getRawX(input_event, event_pointer_index), AMotionEvent_getRawY(input_event, event_pointer_index));
                MousePos = FVector2D(screen_scale.X > 0 ? pos.X / screen_scale.X : pos.X, screen_scale.Y > 0 ? pos.Y / screen_scale.Y : pos.Y);
                break;
          }
        default:
        break;
            }
        }
       default:
       break;
    }
}


	void Render()
	{
		for (int i = 0; i < 128; i++)
		{
			if (PostRenderer::drawlist[i].type != -1)
			{
				//Filled Rect
				if (PostRenderer::drawlist[i].type == 1)
				{
					GUI::drawFilledRect(PostRenderer::drawlist[i].pos, PostRenderer::drawlist[i].size.X, PostRenderer::drawlist[i].size.Y, PostRenderer::drawlist[i].color);
				}
				//TextLeft
				else if (PostRenderer::drawlist[i].type == 2)
				{
					GUI::TextLeft(PostRenderer::drawlist[i].name, PostRenderer::drawlist[i].pos, PostRenderer::drawlist[i].color, PostRenderer::drawlist[i].outline);
				}
				//TextCenter
				else if (PostRenderer::drawlist[i].type == 3)
				{
					GUI::TextCenter(PostRenderer::drawlist[i].name, PostRenderer::drawlist[i].pos, PostRenderer::drawlist[i].color, PostRenderer::drawlist[i].outline);
				}
				//Draw_Line
				else if (PostRenderer::drawlist[i].type == 4)
				{
					Draw_Line(PostRenderer::drawlist[i].from, PostRenderer::drawlist[i].to, PostRenderer::drawlist[i].thickness, PostRenderer::drawlist[i].color);
				}

				PostRenderer::drawlist[i].type = -1;
			}
		}
	}
}
