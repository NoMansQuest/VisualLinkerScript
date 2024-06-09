#ifndef OBJECTSTATE_H__
#define OBJECTSTATE_H__

/// @brief This enum represenst various states a UI object
///        (be it a Load-Mapper arrow, a model or an indicator)
///        could find itself in.
enum class EObjectState {
    Neutral,                // No user interaction ongoing
    Hovering,               // Mouse is hovering over the object
    Dragging,               // User is dragging the object around
    DraggingWithProposal,   // Object is being dragged and new coordinates are being suggested
    CommitRequest           // User dropped the object expecting an update to the model
};

#endif // EOBJECTSTATE_H
