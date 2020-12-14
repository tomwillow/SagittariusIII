#include "StarRender.h"

#include "gl_assist.h"

//星B-V颜色指数为表观RGB颜色
//https://cloud.tencent.com/developer/ask/68777/answer/117942
void bv2rgb(double& r, double& g, double& b, double bv)    // RGB <0,1> <- BV <-0.4,+2.0> [-]
{
	double t;  r = 0.0; g = 0.0; b = 0.0; if (bv < -0.4) bv = -0.4; if (bv > 2.0) bv = 2.0;
	if ((bv >= -0.40) && (bv < 0.00)) { t = (bv + 0.40) / (0.00 + 0.40); r = 0.61 + (0.11 * t) + (0.1 * t * t); }
	else if ((bv >= 0.00) && (bv < 0.40)) { t = (bv - 0.00) / (0.40 - 0.00); r = 0.83 + (0.17 * t); }
	else if ((bv >= 0.40) && (bv < 2.10)) { t = (bv - 0.40) / (2.10 - 0.40); r = 1.00; }
	if ((bv >= -0.40) && (bv < 0.00)) { t = (bv + 0.40) / (0.00 + 0.40); g = 0.70 + (0.07 * t) + (0.1 * t * t); }
	else if ((bv >= 0.00) && (bv < 0.40)) { t = (bv - 0.00) / (0.40 - 0.00); g = 0.87 + (0.11 * t); }
	else if ((bv >= 0.40) && (bv < 1.60)) { t = (bv - 0.40) / (1.60 - 0.40); g = 0.98 - (0.16 * t); }
	else if ((bv >= 1.60) && (bv < 2.00)) { t = (bv - 1.60) / (2.00 - 1.60); g = 0.82 - (0.5 * t * t); }
	if ((bv >= -0.40) && (bv < 0.40)) { t = (bv + 0.40) / (0.40 + 0.40); b = 1.00; }
	else if ((bv >= 0.40) && (bv < 1.50)) { t = (bv - 0.40) / (1.50 - 0.40); b = 1.00 - (0.47 * t) + (0.1 * t * t); }
	else if ((bv >= 1.50) && (bv < 1.94)) { t = (bv - 1.50) / (1.94 - 1.50); b = 0.63 - (0.6 * t * t); }
}

//随机生成一个恒星颜色
glm::vec4 RandStarColor()
{
	double r, g, b;
	bv2rgb(r, g, b, RandFloat(-0.4f, 2.0f));
	return glm::vec4(r, g, b, 1.0f);
}

void StarRender::AddPointStar(float t0)
{
	float x = RandFloat(), y = RandFloat();
	float z = -RandFloat(1, 10000);
	float vz = RandFloat(0, 10000);

	glm::vec3 p = ClipToWorld({ x,y }, z, projection);

	glm::vec4 color;
	switch (RandInt(0, 1))
	{
	case 0:
		color = glm::vec4(1.0f);
		break;
	case 1:
		color = RandStarColor();
		break;
	}
	point_stars.emplace_back(p, vz, t0, Star::ST_POINT, 0, color);
}

void StarRender::AddTexStar(float t0)
{
	float x = RandFloat(), y = RandFloat();//裁剪空间坐标
	float z = -RandFloat(1000, 10000);//z方向生成位置
	float vz;

	glm::vec3 p = ClipToWorld({ x,y }, z, projection);//裁剪空间转世界坐标

	glm::vec4 color;
	unsigned int tex_id;
	int rd = RandInt(0, 1000);
	if (rd >= 980)//2%概率生成非球状贴图恒星
	{
		color = glm::vec4(1.0f);
		tex_id = textures[RandInt(0, textures.size() - 1)].GetId();
		vz = RandFloat(0, 100);
	}
	else
	{
		if (rd >= 975)//0.5%概率生成sos团徽标
		{
			//sos 
			color = glm::vec4(1.0f);
			tex_id = tex_sos.begin()->GetId();
			vz = RandFloat(1000, 5000);
		}
		else//生成球状普通贴图恒星
		{
			//common star
			color = RandStarColor();
			tex_id = textures[3].GetId();
			vz = RandFloat(100, 10000);
		}
	}
	tex_stars.emplace_back(p, vz, t0, Star::ST_TEXTURE, tex_id, color);
}

StarRender::StarRender(int w, int h)
{
	//
	textures.emplace_back(STAR00);
	textures.emplace_back(STAR01);
	textures.emplace_back(STAR02);
	textures.emplace_back(STAR03);
	textures.emplace_back(STAR04);
	tex_sos.emplace_back(SOS_BRIGADE);

	float t0 = (float)GetTickCount() / 1000.0f;
	e.seed(t0);

	projection = glm::perspective(glm::radians(45.0f), (float)w / (float)h, 0.1f, 10000.0f);
	for (int i = 0; i < point_count; ++i)
	{
		AddPointStar(t0);
	}
	for (int i = 0; i < tex_count; ++i)
	{
		AddTexStar(t0);
	}
}

void StarRender::Draw(int w, int h, float t)
{
	EnableGeometry();

	//渲染点恒星
	glBegin(GL_POINTS);
	int point_nun = 0;
	for (auto it = point_stars.begin(); it != point_stars.end();)
	{
		Star& star = *it;
		star.RefreshPos(t);

		glm::vec4 p(star.p, 1.0f);
		p = projection * p;

		float x = p.x, y = p.y, z = p.z, w = p.w;
		bool show = x > -w && x<w&& y>-w && y<w&& z>-w && z < w;
		point_nun += show;


		//printf("%f\n", star.p.z);

		if (!show)
		{
			it = point_stars.erase(it);
		}
		else
		{
			//glm::vec2 p2 = WorldToClip(star.p, projection);

			//	glColor4f(star.color.r,star.color.g,star.color.b,star.color.a);
			//	glVertex2f(p2.x, p2.y);

			glVertex4f(p.x, p.y, p.z, p.w);

			++it;
		}
	}
	glEnd();

	for (int i = 0; i < point_count - point_nun; ++i)
		AddPointStar(t);

	//渲染贴图恒星
	EnableTexture();

	int tex_nun = 0;
	for (auto it = tex_stars.begin(); it != tex_stars.end();)
	{
		Star& star = *it;
		star.RefreshPos(t);

		glm::vec4 p(star.p, 1.0f);
		p = projection * p;

		float x = p.x, y = p.y, z = p.z, w = p.w;
		bool show = x > -w && x<w&& y>-w && y<w&& z>-w && z < w;
		tex_nun += show;


		//printf("%f\n", star.p.z);

		if (!show)
		{
			it = tex_stars.erase(it);
		}
		else
		{
			//以中心点，忽略透视距离渲染图片

			glBindTexture(GL_TEXTURE_2D, star.texture_id);

			float sz = 0.01f;
			if (star.texture_id == tex_sos.begin()->GetId())
				sz = 0.05f;

			glm::vec2 p2 = WorldToClip(star.p, projection);
			//float sz = RandFloat(0.005f,0.02f);
			float left = p2.x - sz, right = p2.x + sz;
			float bottom = p2.y - sz, top = p2.y + sz;

			glColor4f(star.color.r, star.color.g, star.color.b, star.color.a);
			glBegin(GL_QUADS);

			glTexCoord2f(0, 0); glVertex2f(left, bottom);
			glTexCoord2f(1, 0); glVertex2f(right, bottom);
			glTexCoord2f(1, 1); glVertex2f(right, top);
			glTexCoord2f(0, 1); glVertex2f(left, top);

			glEnd();
			++it;
		}
	}

	for (int i = 0; i < tex_count - tex_nun; ++i)
		AddTexStar(t);

}
