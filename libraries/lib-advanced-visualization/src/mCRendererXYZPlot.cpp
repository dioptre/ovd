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
 
#include "mCRendererXYZPlot.hpp"

using namespace Mensia;
using namespace Mensia::AdvancedVisualization;

void CRendererXYZPlot::rebuild(const IRendererContext& rContext)
{
	CRenderer::rebuild(rContext);

	m_bHasDepth=rContext.hasXYZPlotDepth();
	m_ui32PlotDimension=(m_bHasDepth?3:2);
	m_ui32PlotCount=(rContext.getChannelCount()+m_ui32PlotDimension-1) / m_ui32PlotDimension;
	m_vVertex.resize(m_ui32PlotCount);
	float l_f32InverseSampleCount = 1.f / (m_ui32SampleCount < 2 ? 1 : (m_ui32SampleCount - 1));
	for(uint32_t i=0; i<m_ui32PlotCount; i++)
	{
		m_vVertex[i].resize(this->m_ui32SampleCount);
		for(uint32_t j=0; j<this->m_ui32SampleCount; j++)
		{
			m_vVertex[i][j].u = j*l_f32InverseSampleCount;
		}
	}

	m_ui32HistoryIndex=0;
}

void CRendererXYZPlot::refresh(const IRendererContext& rContext)
{
	CRenderer::refresh(rContext);

	if(!m_ui32HistoryCount) return;

	while(m_ui32HistoryIndex < m_ui32HistoryCount)
	{
		uint32_t i, i3, j = m_ui32HistoryIndex % this->m_ui32SampleCount;
		for(i=0; i<m_ui32PlotCount; i++)
		{

			if(m_bHasDepth)
			{
				i3 = i*3;
				m_vVertex[i][j].x = (i3  <m_vHistory.size()?m_vHistory[i3  ][m_ui32HistoryIndex]:0);
				m_vVertex[i][j].y = (i3+1<m_vHistory.size()?m_vHistory[i3+1][m_ui32HistoryIndex]:0);
				m_vVertex[i][j].z = (i3+2<m_vHistory.size()?m_vHistory[i3+2][m_ui32HistoryIndex]:0);
			}
			else
			{
				i3 = i*2;
				m_vVertex[i][j].x = (i3  <m_vHistory.size()?m_vHistory[i3  ][m_ui32HistoryIndex]:0);
				m_vVertex[i][j].y = (i3+1<m_vHistory.size()?m_vHistory[i3+1][m_ui32HistoryIndex]:0);
			}
		}
		m_ui32HistoryIndex++;
	}
}

bool CRendererXYZPlot::render(const IRendererContext& rContext)
{
	if(!rContext.getSelectedCount()) return false;
	if(!m_vVertex.size()) return false;
	if(!m_ui32HistoryCount) return false;

	::glPointSize(5);

	if(m_bHasDepth)
	{
		float d=3.5;

		::glMatrixMode(GL_PROJECTION);
		::glPushMatrix();
		::glLoadIdentity();
		::gluPerspective(60, rContext.getAspect(), .01, 100);
		::glTranslatef(0, 0, -d);
		::glRotatef(rContext.getRotationX()*10, 1, 0, 0);
		::glRotatef(rContext.getRotationY()*10, 0, 1, 0);
	}

	::glMatrixMode(GL_TEXTURE);
	::glPushMatrix();
	::glLoadIdentity();

	::glMatrixMode(GL_MODELVIEW);
	::glTranslatef(m_bHasDepth?0:0.5f, m_bHasDepth?0:0.5f, 0);
	::glScalef(rContext.getZoom(), rContext.getZoom(), rContext.getZoom());

	if(rContext.isAxisDisplayed())
	{
		if(m_bHasDepth)
		{
			this->draw3DCoordinateSystem();
		}
		else
		{
			this->draw2DCoordinateSystem();
		}
	}

	uint32_t n = m_ui32SampleCount;
	uint32_t d = (m_ui32HistoryIndex%m_ui32SampleCount);

	::glEnableClientState(GL_VERTEX_ARRAY);
	::glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	for(uint32_t i=0; i<m_ui32PlotCount; i++)
	{
		::glPushMatrix();
		::glScalef(rContext.getScale(), rContext.getScale(), rContext.getScale());

		::glVertexPointer(m_ui32PlotDimension, GL_FLOAT, sizeof(CVertex), &m_vVertex[i][0].x);
		::glTexCoordPointer(1, GL_FLOAT, sizeof(CVertex), &m_vVertex[i][n - d].u);
		::glDrawArrays(GL_POINTS, 0, d);

		::glVertexPointer(m_ui32PlotDimension, GL_FLOAT, sizeof(CVertex), &m_vVertex[i][d].x);
		::glTexCoordPointer(1, GL_FLOAT, sizeof(CVertex), &m_vVertex[i][0].u);
		::glDrawArrays(GL_POINTS, 0, (m_ui32HistoryIndex>n ? n : m_ui32HistoryIndex) - d);

		::glPopMatrix();
	}
	::glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	::glDisableClientState(GL_VERTEX_ARRAY);

	::glMatrixMode(GL_TEXTURE);
	::glPopMatrix();

	if(m_bHasDepth)
	{
		::glMatrixMode(GL_PROJECTION);
		::glPopMatrix();
	}

	::glMatrixMode(GL_MODELVIEW);

	return true;
}
