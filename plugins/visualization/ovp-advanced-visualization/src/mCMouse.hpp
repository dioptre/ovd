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

#pragma once

#include <mensia/advanced-visualization.h>

#include <map>

namespace Mensia
{
	namespace AdvancedVisualization
	{
		class CBoxAlgorithmViz;

		class CMouse
		{
		public:

			CMouse(CBoxAlgorithmViz& rBoxAlgorithmViz);
			void mouseButton(IRendererContext& rContext, int32_t x, int32_t y, int32_t button, int status);
			void mouseMotion(IRendererContext& rContext, int32_t x, int32_t y);
			bool hasButtonPressed(void);

			CBoxAlgorithmViz& m_rBoxAlgorithmViz;
			std::map < int32_t, int > m_vButton;
			int32_t m_i32MouseX;
			int32_t m_i32MouseY;
		};
	};
};

