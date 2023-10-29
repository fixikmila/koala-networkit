

#include <list>
#include <set>

#include <graph/GraphTools.hpp>
#include <cograph_recognition/CographRecognition.hpp>

namespace Koala {

CographRecognition::CographRecognition(NetworKit::Graph &graph)
    : graph(std::make_optional(graph)), is_complement_reducible(State::UNKNOWN) {

     }

bool CographRecognition::isComplementReducible() const {
    assureFinished();
    return is_complement_reducible == State::COMPLEMENT_REDUCIBLE;
}

CographRecognition::State CographRecognition::getState() const {
    assureFinished();
    return is_complement_reducible;
}

void CographRecognition::run() {
    hasRun = true;
    if (is_complement_reducible != State::UNKNOWN) {
        return;
    }
    Cograph__Recognition(graph);
    if (is_complement_reducible != State::UNKNOWN) {
            return;
    }

    is_complement_reducible = State::COMPLEMENT_REDUCIBLE;
}





}  // namespace Koala
