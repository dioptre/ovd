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

#ifndef __OpenViBEPlugins_CRulerRightFrequency_H__
#define __OpenViBEPlugins_CRulerRightFrequency_H__

#include "../mIRuler.hpp"
#include "../m_VisualizationTools.hpp"

namespace Mensia
{
	namespace AdvancedVisualization
	{
		class CRulerRightFrequency : public IRuler
		{
		public:

			virtual void renderRight(::GtkWidget* pWidget)
			{
				float l_fScale=(float)m_pRendererContext->getSpectrumFrequencyRange();
				if(m_fLastScale!=l_fScale) { m_vRange=this->split_range(0, l_fScale); m_fLastScale=l_fScale; }

				gint w, h, y;
				gint lw, lh;

				uint32_t l_ui32ChannelCount=m_pRendererContext->getSelectedCount();
				for(uint32_t i=0; i<l_ui32ChannelCount; i++)
				{
					::gdk_drawable_get_size(pWidget->window, &w, &h);
					::GdkGC* l_pDrawGC=gdk_gc_new(pWidget->window);
					for(it=m_vRange.begin(); it!=m_vRange.end(); it++)
					{
						y=gint((i+*it/l_fScale)*(h*1.f/l_ui32ChannelCount));
						::PangoLayout* l_pPangoLayout=::gtk_widget_create_pango_layout(pWidget, this->getLabel(*it).c_str());
						::pango_layout_get_size(l_pPangoLayout, &lw, &lh);
						lw/=PANGO_SCALE;
						lh/=PANGO_SCALE;
						::gdk_draw_layout(pWidget->window, l_pDrawGC, 8, h-y-lh/2, l_pPangoLayout);
						::gdk_draw_line(pWidget->window, l_pDrawGC, 0, h-y, 3, h-y);
						g_object_unref(l_pPangoLayout);
					}
					g_object_unref(l_pDrawGC);
				}
			}

			float m_fLastScale;
			std::vector < double > m_vRange;
			std::vector < double >::iterator it;
		};
	};
};

#endif // __OpenViBEPlugins_CRulerRightFrequency_H__
