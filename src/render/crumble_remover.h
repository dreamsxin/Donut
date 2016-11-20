#ifndef CRUMBLE_REMOVER_H
#define CRUMBLE_REMOVER_H

// Library includes
#include "render_request.h"
#include "core/frustum.h"
#include "core/box3.h"

namespace Donut
{
	// This class is able to remove request that are considered too small
	class TCrumbleRemover
	{
	public:
		// Cst and Dst
		TCrumbleRemover();
		~TCrumbleRemover();

		void Process(std::vector<TRenderRequest>& _requests, const Matrix4& _view, const TFrustum& _frusutm);

	protected:
		std::vector<char> m_discardArray;
		std::vector<TBox3> m_wsBoxes;
	};
}
#endif // CRUMBLE_REMOVER_H