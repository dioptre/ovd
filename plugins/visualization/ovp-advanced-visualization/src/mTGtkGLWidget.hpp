/*********************************************************************
 * Software License Agreement (AGPL-3 License)
 *
 * OpenViBE Designer
 * Based on OpenViBE V1.1.0, Copyright (C) Inria, 2006-2015
 * Copyright (C) Inria, 2015-2017,V1.0
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3,
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __OpenViBEPlugins_TGtkGLWidget_H__
#define __OpenViBEPlugins_TGtkGLWidget_H__

#include "m_defines.hpp"
#include "m_VisualizationTools.hpp"
#include "m_GtkGL.hpp"

#include <openvibe/ov_all.h>
#include <visualization-toolkit/ovviz_all.h>

#if defined TARGET_OS_Windows
#include <windows.h>
#endif

#include <gtk/gtk.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <map>
#include <string>

// #define __DIRECT_RENDER__

namespace Mensia
{
	namespace AdvancedVisualization
	{
		template <class TBox>
		class TGtkGLWidget
		{
		public:

			TGtkGLWidget(void)
				:m_pWidget(NULL)
				,m_pLeft(NULL)
				,m_pRight(NULL)
				,m_pBottom(NULL)
				,m_pTimeoutSource(NULL)
				,m_pBox(NULL)
				,m_ui32TextureId(0)
			{
			}

			virtual ~TGtkGLWidget(void)
			{
				if(m_pWidget)
				{
					if(m_ui32TextureId)
					{
						GtkGL::preRender(m_pWidget);
						::glDeleteTextures(1, &m_ui32TextureId);
						GtkGL::postRender(m_pWidget);
					}
					if(m_pTimeoutSource)
					{
						::g_source_destroy(m_pTimeoutSource);
					}
					GtkGL::uninitialize(m_pWidget);
				}
			}

			virtual void initialize(TBox& rBox, ::GtkWidget* pWidget, ::GtkWidget* pLeft, ::GtkWidget* pRight, ::GtkWidget* pBottom)
			{
				GtkGL::initialize(pWidget);
				{
					m_pBox=&rBox;
					m_pWidget=pWidget;
					m_pLeft=pLeft;
					m_pRight=pRight;
					m_pBottom=pBottom;

					::g_signal_connect(pWidget, "configure-event", G_CALLBACK(TGtkGLWidget<TBox>::__configure_cb), m_pBox);
					::g_signal_connect(pWidget, "expose-event", G_CALLBACK(TGtkGLWidget<TBox>::__expose_cb), m_pBox);
					::g_signal_connect(pWidget, "button-press-event", G_CALLBACK(TGtkGLWidget<TBox>::__mouse_button_cb), m_pBox);
					::g_signal_connect(pWidget, "button-release-event", G_CALLBACK(TGtkGLWidget<TBox>::__mouse_button_cb), m_pBox);
					::g_signal_connect(pWidget, "motion-notify-event", G_CALLBACK(TGtkGLWidget<TBox>::__motion_notify_cb), m_pBox);
					::g_signal_connect(pWidget, "enter-notify-event", G_CALLBACK(TGtkGLWidget<TBox>::__enter_notify_cb), m_pBox);
					::g_signal_connect(gtk_widget_get_parent(pWidget), "key-press-event", G_CALLBACK(TGtkGLWidget<TBox>::__key_press_cb), m_pBox);
					::g_signal_connect(gtk_widget_get_parent(pWidget), "key-release-event", G_CALLBACK(TGtkGLWidget<TBox>::__key_release_cb), m_pBox);

					::g_signal_connect_after(pLeft, "expose-event", G_CALLBACK(TGtkGLWidget<TBox>::__expose_left_cb), m_pBox);
					::g_signal_connect_after(pRight, "expose-event", G_CALLBACK(TGtkGLWidget<TBox>::__expose_right_cb), m_pBox);
					::g_signal_connect_after(pBottom, "expose-event", G_CALLBACK(TGtkGLWidget<TBox>::__expose_bottom_cb), m_pBox);

					m_pTimeoutSource=g_timeout_source_new(250); // timeouts every 50 ms
					g_source_set_priority(m_pTimeoutSource, G_PRIORITY_LOW);
					g_source_set_callback(m_pTimeoutSource, GSourceFunc(TGtkGLWidget<TBox>::__timeout_redraw), m_pBox, NULL);
					g_source_attach(m_pTimeoutSource, NULL);

					::gtk_widget_queue_resize(pWidget);
				}
			}

			virtual void redrawTopLevelWindow(bool bImmediate=false)
			{
				::GtkWidget* l_pTopLevelWidget=gtk_widget_get_toplevel(m_pWidget);
				if(l_pTopLevelWidget != NULL)
				{
					if(bImmediate)
					{
						gdk_window_process_updates(l_pTopLevelWidget->window, false);
						gtk_widget_queue_draw(l_pTopLevelWidget);
					}
					else
					{
						gdk_window_invalidate_rect(l_pTopLevelWidget->window, NULL, true);
					}
				}
			}

			virtual void redraw(bool bImmediate=false)
			{
				if(bImmediate)
				{
					gdk_window_process_updates(m_pWidget->window, false);
					gtk_widget_queue_draw(m_pWidget);
				}
				else
				{
					gdk_window_invalidate_rect(m_pWidget->window, NULL, true);
				}
			}

			virtual void redrawLeft(bool bImmediate=false)
			{
				if(bImmediate)
				{
					gdk_window_process_updates(m_pLeft->window, false);
					gtk_widget_queue_draw(m_pLeft);
				}
				else
				{
					gdk_window_invalidate_rect(m_pLeft->window, NULL, true);
				}
			}

			virtual void redrawRight(bool bImmediate=false)
			{
				if(bImmediate)
				{
					gdk_window_process_updates(m_pRight->window, false);
					gtk_widget_queue_draw(m_pRight);
				}
				else
				{
					gdk_window_invalidate_rect(m_pRight->window, NULL, true);
				}
			}

			virtual void redrawBottom(bool bImmediate=false)
			{
				if(bImmediate)
				{
					gdk_window_process_updates(m_pBottom->window, false);
					gtk_widget_queue_draw(m_pBottom);
				}
				else
				{
					gdk_window_invalidate_rect(m_pBottom->window, NULL, true);
				}
			}

		public:

			virtual void setPointSmoothingActive(bool bActive=false)
			{
				if(bActive)
				{
					::glEnable(GL_POINT_SMOOTH);
				}
				else
				{
					::glDisable(GL_POINT_SMOOTH);
				}
			}

			virtual uint32_t createTexture(const std::string& sValue)
			{
#define M_GRADIENT_SIZE 128

				if(m_ui32TextureId==0)
				{
					std::string l_sValue=(sValue==""?"0:0,0,100; 25:0,100,100; 50:0,49,0; 75:100,100,0; 100:100,0,0":sValue);

					uint32_t i;

					OpenViBE::CMatrix m_oGradientBase;
					OpenViBE::CMatrix m_oGradient;
					OpenViBEVisualizationToolkit::Tools::ColorGradient::parse(m_oGradientBase, l_sValue.c_str());
					OpenViBEVisualizationToolkit::Tools::ColorGradient::interpolate(m_oGradient, m_oGradientBase, M_GRADIENT_SIZE);

					float l_vTexture[M_GRADIENT_SIZE][3];
					for(i=0; i<M_GRADIENT_SIZE; i++)
					{
						l_vTexture[i][0]=float(m_oGradient[i*4+1]*.01);
						l_vTexture[i][1]=float(m_oGradient[i*4+2]*.01);
						l_vTexture[i][2]=float(m_oGradient[i*4+3]*.01);
					}

					::glGenTextures(1, &m_ui32TextureId);
					::glBindTexture(GL_TEXTURE_1D, m_ui32TextureId);
					::glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // GL_LINEAR);
					::glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // LINEAR_MIPMAP_LINEAR);
					::glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP);
					::glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP);
//					::glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
//					::glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, __SIZE__, 0, GL_RGB, GL_UNSIGNED_BYTE, l_vTexture);
					::gluBuild1DMipmaps(GL_TEXTURE_1D, GL_RGB, M_GRADIENT_SIZE, GL_RGB, GL_FLOAT, l_vTexture);
				}

#undef M_GRADIENT_SIZE

				return m_ui32TextureId;
			}

		protected:

			::GtkWidget* m_pWidget;
			::GtkWidget* m_pLeft;
			::GtkWidget* m_pRight;
			::GtkWidget* m_pBottom;
			::GSource* m_pTimeoutSource;

			TBox* m_pBox;

			uint32_t m_ui32TextureId;

		private:


			static gboolean __timeout_redraw(TBox* pBox)
			{
				pBox->redraw();
				return TRUE;
			}

			static gboolean __configure_cb(::GtkWidget* pWidget, ::GdkEventConfigure* pEvent, TBox* pBox)
			{
				GtkGL::preRender(pWidget);

				::glViewport(0, 0, pWidget->allocation.width, pWidget->allocation.height);

				pBox->reshape(pWidget->allocation.width, pWidget->allocation.height);

				GtkGL::postRender(pWidget);

				return TRUE;
			}

			static gboolean __expose_cb(::GtkWidget* pWidget, ::GdkEventExpose* pEvent, TBox* pBox)
			{
				float d=1.f;
				float dx=d/(pWidget->allocation.width-d);
				float dy=d/(pWidget->allocation.height-d);

				GtkGL::preRender(pWidget);

				::glMatrixMode(GL_TEXTURE);
				::glLoadIdentity();
				::glTranslatef(0.5, 0, 0);

				::glMatrixMode(GL_PROJECTION);
				::glLoadIdentity();
				::gluOrtho2D(0-dx, 1+dx, 0-dy, 1+dy);

				::glMatrixMode(GL_MODELVIEW);
				::glLoadIdentity();

				::glEnable(GL_POLYGON_OFFSET_FILL);
				::glPolygonOffset(1.0, 1.0);

				::glEnable(GL_LINE_SMOOTH);
				::glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
				::glLineWidth(1);

				::glEnable(GL_BLEND);
				::glBlendFunc(GL_SRC_ALPHA, GL_ONE);

				::glEnable(GL_TEXTURE_1D);

				::glDisable(GL_DEPTH_TEST);
				::glClearDepth(100);
				::glClearColor(0, 0, 0, 0);
				::glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
				::glColor3f(1, 1, 1);

				// Lighting
				float fAmbient = 0.0f;
				float fDiffuse = 1.0f;
				float fSpecular = 1.0f;
				::GLfloat l_vAmbient[] = { fAmbient, fAmbient, fAmbient, 1 };
				::GLfloat l_vDiffuse[] = { fDiffuse, fDiffuse, fDiffuse, 1 };
				::GLfloat l_vSpecular[] = { fSpecular, fSpecular, fSpecular, 1 };
				::GLfloat l_vPosition0[] = { 3, 1, 2, 1 };
				::GLfloat l_vPosition1[] = { -3, 0, -2, 1 };
				::glLightfv(GL_LIGHT0, GL_AMBIENT, l_vAmbient);
				::glLightfv(GL_LIGHT0, GL_DIFFUSE, l_vDiffuse);
				::glLightfv(GL_LIGHT0, GL_SPECULAR, l_vSpecular);
				::glLightfv(GL_LIGHT0, GL_POSITION, l_vPosition0);
				::glLightfv(GL_LIGHT1, GL_AMBIENT, l_vAmbient);
				::glLightfv(GL_LIGHT1, GL_DIFFUSE, l_vDiffuse);
				::glLightfv(GL_LIGHT1, GL_SPECULAR, l_vSpecular);
				::glLightfv(GL_LIGHT1, GL_POSITION, l_vPosition1);
				::glShadeModel(GL_SMOOTH);
				::glEnable(GL_LIGHT0);
				::glEnable(GL_LIGHT1);
				::glEnable(GL_COLOR_MATERIAL);
				::glDisable(GL_LIGHTING);

				pBox->draw();

				GtkGL::postRender(pWidget);

				return TRUE;
			}

			static gboolean __enter_notify_cb(::GtkWidget* pWidget, ::GdkEventCrossing* pEvent, TBox* pBox)
			{
				pBox->redraw();
//				pBox->request();
//				pBox->m_bRedrawNeeded=true;
				return TRUE;
			}

			static gboolean __expose_left_cb(::GtkWidget* pWidget, ::GdkEventExpose* pEvent, TBox* pBox)
			{
				pBox->drawLeft();
				return TRUE;
			}

			static gboolean __expose_right_cb(::GtkWidget* pWidget, ::GdkEventExpose* pEvent, TBox* pBox)
			{
				pBox->drawRight();
				return TRUE;
			}

			static gboolean __expose_bottom_cb(::GtkWidget* pWidget, ::GdkEventExpose* pEvent, TBox* pBox)
			{
				pBox->drawBottom();
				return TRUE;
			}

			static gboolean __mouse_button_cb(::GtkWidget* pWidget, ::GdkEventButton* pEvent, TBox* pBox)
			{
				int l_iStatus=0;
				switch(pEvent->type)
				{
					case GDK_BUTTON_PRESS: l_iStatus=1; break;
					case GDK_2BUTTON_PRESS: l_iStatus=2; break;
					case GDK_3BUTTON_PRESS: l_iStatus=3; break;
					default: break;
				}
				pBox->mouseButton(int32_t(pEvent->x), int32_t(pEvent->y), pEvent->button, l_iStatus);
				pBox->draw();
				return TRUE;
			}

			static gboolean __motion_notify_cb(::GtkWidget* pWidget, ::GdkEventMotion* pEvent, TBox* pBox)
			{
				pBox->mouseMotion(int32_t(pEvent->x), int32_t(pEvent->y));
				return TRUE;
			}

			static gboolean __key_press_cb(::GtkWidget* pWidget, ::GdkEventKey* pEvent, TBox* pBox)
			{
				pBox->keyboard(0, 0, /*pEvent->x, pEvent->y,*/ pEvent->keyval, true);
				return TRUE;
			}

			static gboolean __key_release_cb(::GtkWidget* pWidget, ::GdkEventKey* pEvent, TBox* pBox)
			{
				pBox->keyboard(0, 0, /*pEvent->x, pEvent->y,*/ pEvent->keyval, false);
				return TRUE;
			}
		};
	};
};

#endif // __OpenViBEPlugins_TGtkGLWidget_H__
