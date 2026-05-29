#include "/home/codeleaded/System/Static/Library/Geometry.h"
#include "/home/codeleaded/System/Static/Library/TransformedView.h"
#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"

Vec2* selected = NULL;
Triangle tri;
TransformedView tv;

char Vec2_Side(Vec2 s,Vec2 t,Vec2 p){
    const Vec2 d = Vec2_Sub(t,s);
    const Vec2 pp = Vec2_PerpA(d);
    const Vec2 vp = Vec2_Sub(p,s);
    const float dp = Vec2_Dot(vp,pp);
    //RenderLine(TransformedView_WorldScreenPos(&tv,s),TransformedView_WorldScreenPos(&tv,Vec2_Add(s,pp)),WHITE,1.0f);
    //RenderLine(TransformedView_WorldScreenPos(&tv,s),TransformedView_WorldScreenPos(&tv,Vec2_Add(s,vp)),WHITE,1.0f);
    return dp >= 0.0f;
}
char Triangle_Point(Triangle t,Vec2 p){
    const char AB = Vec2_Side(t.p1,t.p2,p);
    const char BC = Vec2_Side(t.p2,t.p3,p);
    const char CA = Vec2_Side(t.p3,t.p1,p);
    return AB==BC && BC==CA;
}

void Setup(AlxWindow* w){
    tv = TransformedView_New((Vec2){ GetWidth(),GetHeight() });
	tri = Triangle_New((Vec2){ 0.5f,0.0f },(Vec2){ 1.0f,1.0f },(Vec2){ 0.0f,1.0f });
}
void Update(AlxWindow* w){
	TransformedView_HandlePanZoom(&tv,window.Strokes,GetMouse());

    const float rs = 4.0f;
	const float r = TransformedView_ScreenWorldLX(&tv,rs);

	if(Stroke(ALX_MOUSE_L).PRESSED){
		selected = NULL;

		const Vec2 m = TransformedView_ScreenWorldPos(&tv,GetMouse());

		for(int i = 0;i<3;i++){
			Vec2* p = (Vec2*)(&tri) + i;
			const Vec2 d = Vec2_Sub(*p,m);
			
			if(Vec2_Mag2(d) < r * r){
				selected = p;
			}
		}
	}
	if(Stroke(ALX_MOUSE_L).DOWN){
		if(selected){
			const Vec2 m = TransformedView_ScreenWorldPos(&tv,GetMouse());
			*selected = m;
		}
	}

	Clear(BLACK);

    Vec2 m = TransformedView_ScreenWorldPos(&tv,GetMouse());

    const char InTri = Triangle_Point(tri,m);
    //const char InTri = Overlap_Triangle_Point(tri,m);
    const Pixel c = InTri ? GREEN : RED;
    const Vec2 s1 = TransformedView_WorldScreenPos(&tv,tri.p1);
    const Vec2 s2 = TransformedView_WorldScreenPos(&tv,tri.p2);
    const Vec2 s3 = TransformedView_WorldScreenPos(&tv,tri.p3);
	RenderTriangleWire(s1,s2,s3,c,1.0f);
}
void Delete(AlxWindow* w){
	
}

int main(){
    if(Create("Triangles",1920,1080,1,1,Setup,Update,Delete))
        Start();
    return 0;
}