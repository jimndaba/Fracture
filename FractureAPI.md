# Fracture Scripting API

## Components

### Tag Component
---

#### Member

`Transform.Name`: The Entity Name Tag 

`Transform.ID`  : The Entity ID

#### Functions


### Transform Component
---

#### Member

`Transform.Position`: World Space position 

`Transform.Scale`  : Scale

`Transform.Rotation`  : Rotation

`Transform.RID`  : The Entity ID

#### Functions

`Transform::Translate(vec3 value)` : Translate Transform

`Transform::LookAt(vec3 value,vec3 up)` : Look at point *value* from *up* vector

`Transform::SetPosition(vec3 value)` : Set position to *value*

`Transform::SetScale(vec3 value)` : Set Scale to *value*

`Transform::SetRotation(vec3 value)` : Set Rotation to *value*


