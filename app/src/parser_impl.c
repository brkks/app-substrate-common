/*******************************************************************************
*  (c) 2019 Zondax GmbH
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
********************************************************************************/

#include <zxmacros.h>
#include <zxformat.h>
#include "parser_impl.h"
#include "parser_txdef.h"
#include "coin.h"
#include "crypto_helper.h"
#include "bignum.h"
#include "substrate_types.h"
#include "substrate_dispatch.h"

extern uint16_t __address_type;

parser_error_t _readTx(parser_context_t *c, parser_tx_t *v) {
    CHECK_INPUT()

    // Reverse parse to retrieve spec before forward parsing
    CHECK_ERROR(_checkVersions(c)) // TODO_GRANT: need to remove?
    CHECK_ERROR(_readTxHash(c, &v->txHash))
    // CHECK_ERROR(_readEra(c, &v->era))
    // CHECK_ERROR(_readCompactIndex(c, &v->nonce))
    // CHECK_ERROR(_readCompactBalance(c, &v->tip))
    // CHECK_ERROR(_readUInt32(c, &v->specVersion))
    // CHECK_ERROR(_readUInt32(c, &v->transactionVersion))
    CHECK_ERROR(_readGenesisHash(c, &v->genesisHash))
    bool isKnownChain = _readKnownChainType(v);
    
    if (isKnownChain) {
        // TODO_GRANT: is it reuired ti shift buffer counter?
        CHECK_ERROR(_readCallIndex(c, &v->callIndex))
        parser_error_t err = _readMethod(c, v); // TODO_GRANT
    }
    // CHECK_ERROR(_readHash(c, &v->blockHash))

    if (c->offset < c->bufferLen) {
        return parser_unexpected_unparsed_bytes;
    }
    
    if (c->offset > c->bufferLen) {
        return parser_unexpected_buffer_end;
    }
    
    __address_type = _detectAddressType(c);

    return parser_ok;
}
