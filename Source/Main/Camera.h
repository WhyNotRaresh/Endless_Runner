#pragma once
#include <include/glm.h>
#include <include/math.h>
#include "Player/Player.h"

namespace Camera
{
	class CameraClass
	{
	public:
		CameraClass(const glm::vec3& position, const glm::vec3& forward, const glm::vec3& up, float maxRenderDistance, float angleOX)
		{
			this->position = position;
			this->forward = forward;
			this->up = up;
			right = glm::cross(forward, up);
			renderDistance = maxRenderDistance;
			angle = angleOX;
			Rotate_OX(angle);
			isFirstPerson = false;
		}

		~CameraClass()
		{ }

		void ChangePerson() {
			if (isFirstPerson) {
				Rotate_OX(angle);
			}
			else {
				Rotate_OX(-angle);
			}

			isFirstPerson = !isFirstPerson;
		}

		glm::mat4 GetViewMatrix()
		{
			return glm::lookAt(GetPosition(), GetPosition() + forward, up);
		}

		glm::vec3 GetPosition()
		{
			if (isFirstPerson) {
				Player* p = Player::GetInstance();
				return p->GetPosition() + glm::vec3(0, p->GetRadius(), 0);
			}
			else
				return position;
		}

		void SetProjectionMatrix(glm::highp_mat4 projectionM)
		{
			projectionMatrix = projectionM;
		}

		glm::highp_mat4 GetProjectionMatrix()
		{
			return projectionMatrix;
		}

		float GetRenderDistance()
		{
			return renderDistance;
		}

		float GetAngleOXRotation()
		{
			return angle;
		}

		bool IsFirstPerson()
		{
			return isFirstPerson;
		}

	private:

		void Rotate_OX(float angle)
		{
			forward = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), angle, right) * glm::vec4(forward, 1)));
			up = glm::normalize(glm::cross(right, forward));
		}

		glm::highp_mat4 projectionMatrix;
		glm::vec3 position;
		glm::vec3 forward;
		glm::vec3 right;
		glm::vec3 up;
		float renderDistance;
		float angle;
		bool isFirstPerson;
	};
}