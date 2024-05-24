﻿// Copyright Timothé Lapetite 2024
// Released under the MIT license https://opensource.org/license/MIT/

#pragma once

#include "CoreMinimal.h"
#include "PCGEx.h"

namespace PCGExGeo
{
	struct PCGEXTENDEDTOOLKIT_API FTriangle
	{
		int32 Vtx[3];

		explicit FTriangle(const int32 A, const int32 B, const int32 C)
		{
			Vtx[0] = A;
			Vtx[1] = B;
			Vtx[2] = C;
			std::sort(std::begin(Vtx), std::end(Vtx));
		}

		explicit FTriangle(const int (&ABC)[3])
			: FTriangle(ABC[0], ABC[1], ABC[2])
		{
		}

		void Set(const int ABC[3])
		{
			Vtx[0] = ABC[0];
			Vtx[1] = ABC[1];
			Vtx[2] = ABC[2];
		}

		bool Equals(const int32 A, const int32 B, const int32 C) const
		{
			return Vtx[0] == A && Vtx[1] == B && Vtx[2] == C;
		}

		bool ContainsEdge(const uint64 Edge) const
		{
			uint32 A;
			uint32 B;
			PCGEx::H64(Edge, A, B);
			return
				(Vtx[0] == A && Vtx[1] == B) ||
				(Vtx[1] == A && Vtx[2] == B) ||
				(Vtx[0] == A && Vtx[2] == B);
		}

		bool ContainsEdge(const int32 A, const int32 B) const
		{
			return
				(Vtx[0] == A && Vtx[1] == B) ||
				(Vtx[1] == A && Vtx[2] == B) ||
				(Vtx[0] == A && Vtx[2] == B);
		}

		void GetLongestEdge(const TArrayView<FVector>& Positions, uint64& Edge) const
		{
			const double L[3] = {
				FVector::DistSquared(Positions[Vtx[0]], Positions[Vtx[1]]),
				FVector::DistSquared(Positions[Vtx[0]], Positions[Vtx[2]]),
				FVector::DistSquared(Positions[Vtx[1]], Positions[Vtx[2]])
			};

			if (L[0] > L[1] && L[0] > L[2]) { Edge = PCGEx::H64U(L[0], L[1]); }
			else if (L[1] > L[0] && L[1] > L[2]) { Edge = PCGEx::H64U(L[0], L[2]); }
			else { Edge = PCGEx::H64U(L[1], L[2]); }
		}

		void GetLongestEdge(const TArrayView<FVector2D>& Positions, uint64& Edge) const
		{
			const double L[3] = {
				FVector2D::DistSquared(Positions[Vtx[0]], Positions[Vtx[1]]),
				FVector2D::DistSquared(Positions[Vtx[0]], Positions[Vtx[2]]),
				FVector2D::DistSquared(Positions[Vtx[1]], Positions[Vtx[2]])
			};

			if (L[0] > L[1] && L[0] > L[2]) { Edge = PCGEx::H64U(L[0], L[1]); }
			else if (L[1] > L[0] && L[1] > L[2]) { Edge = PCGEx::H64U(L[0], L[2]); }
			else { Edge = PCGEx::H64U(L[1], L[2]); }
		}

		bool operator==(const FTriangle& Other) const
		{
			return Vtx[0] == Vtx[0] && Vtx[1] == Vtx[1] && Vtx[2] == Vtx[2];
		}
	};
}