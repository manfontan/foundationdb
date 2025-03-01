/*
 * BlobMigratorInterface.h
 *
 * This source file is part of the FoundationDB open source project
 *
 * Copyright 2013-2022 Apple Inc. and the FoundationDB project authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef FDBSERVER_BLOBMIGRATORINTERFACE_H
#define FDBSERVER_BLOBMIGRATORINTERFACE_H
#pragma once

#include "fdbclient/StorageServerInterface.h"
#include "fdbclient/FDBTypes.h"
#include "fdbrpc/Locality.h"
#include "fdbrpc/fdbrpc.h"

struct BlobMigratorInterface {
	constexpr static FileIdentifier file_identifier = 869199;
	RequestStream<struct HaltBlobMigratorRequest> haltBlobMigrator;
	RequestStream<ReplyPromise<Void>> waitFailure;
	LocalityData locality;
	UID uniqueID;

	BlobMigratorInterface() {}
	BlobMigratorInterface(const struct LocalityData& l, UID id) : uniqueID(id), locality(l) {}

	void initEndpoints() {}
	UID id() const { return uniqueID; }
	NetworkAddress address() const { return waitFailure.getEndpoint().getPrimaryAddress(); }
	bool operator==(const BlobMigratorInterface& r) const { return id() == r.id(); }
	bool operator!=(const BlobMigratorInterface& r) const { return !(*this == r); }

	template <class Archive>
	void serialize(Archive& ar) {
		// StorageServerInterface::serialize(ar);
		serializer(ar, waitFailure, haltBlobMigrator, locality, uniqueID);
	}
};

struct HaltBlobMigratorRequest {
	constexpr static FileIdentifier file_identifier = 4980139;
	UID requesterID;
	ReplyPromise<Void> reply;

	HaltBlobMigratorRequest() {}
	explicit HaltBlobMigratorRequest(UID uid) : requesterID(uid) {}

	template <class Ar>
	void serialize(Ar& ar) {
		serializer(ar, requesterID, reply);
	}
};

#endif
