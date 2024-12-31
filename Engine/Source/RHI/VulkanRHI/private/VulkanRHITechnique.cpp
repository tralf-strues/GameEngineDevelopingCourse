#include <VulkanRHITechnique.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		VulkanRHITechnique::VulkanRHITechnique(
			VulkanRHIDevice::Ptr device,
			const ShaderInfo& shaderInfo,
			const InputLayout& inputLayout,
			const RootSignature& rootSignature,
			ShaderModuleList&& shaderModuleList
		) 
			: RHITechnique(shaderInfo, inputLayout, rootSignature)
			, m_Device(device)
			, m_ShaderModuleList(std::move(shaderModuleList))
		{
			FillInputAssemblyInfo();
			CreateDescriptorSet();
			CreatePipelineLayout();
		}

		VulkanRHITechnique::~VulkanRHITechnique()
		{
			for (auto& [type, shaderModule] : m_ShaderModuleList)
			{
				if (shaderModule != VK_NULL_HANDLE)
				{
					vkDestroyShaderModule(m_Device->GetHandle(), shaderModule, nullptr);
					shaderModule = VK_NULL_HANDLE;
				}
			}

			m_DescriptorSet = VK_NULL_HANDLE;

			if (m_DescriptorPool != VK_NULL_HANDLE)
			{
				vkDestroyDescriptorPool(m_Device->GetHandle(), m_DescriptorPool, nullptr);
				m_DescriptorPool = VK_NULL_HANDLE;
			}

			if (m_PipelineLayout != VK_NULL_HANDLE)
			{
				vkDestroyPipelineLayout(m_Device->GetHandle(), m_PipelineLayout, nullptr);
				m_PipelineLayout = VK_NULL_HANDLE;
			}

			if (m_DescriptorSetLayout != VK_NULL_HANDLE)
			{
				vkDestroyDescriptorSetLayout(m_Device->GetHandle(), m_DescriptorSetLayout, nullptr);
				m_DescriptorSetLayout = VK_NULL_HANDLE;
			}
		}

		RenderNativeObject VulkanRHITechnique::GetNativeObject()
		{
			assert(0 && "VulkanRHITechnique can't have the native object");
			return nullptr;
		}

		void VulkanRHITechnique::FillInputAssemblyInfo()
		{
			InputBindingList bindings;
			InputAttributeList attributes;
			uint32_t prevInputSlot = UINT_MAX;
			uint32_t location = 0;

			for (const RHITechnique::InputLayoutDescription inputDesc : GetInputLayout())
			{
				// Not supported in Vulkan
				assert(inputDesc.InstanceDataStepRate == 0);

				// New binding and with reset offset, starting a new input slot description
				if (inputDesc.InputSlot != prevInputSlot)
				{
					prevInputSlot = inputDesc.InputSlot;

					bindings.push_back({
						.binding = inputDesc.InputSlot,
						.stride = 0,
						.inputRate = ConvertToVkVertexInputRate(inputDesc.InputSlotClass),
					});
				}

				attributes.push_back({
					.location = location++,
					.binding = inputDesc.InputSlot,
					.format = ConvertToVkFormat(inputDesc.Format),
					.offset = bindings.back().stride,
				});

				bindings.back().stride += GetFormatSize(inputDesc.Format);
			}

			m_InputBindingList = bindings;
			m_InputAttributeList = attributes;
		}

		void VulkanRHITechnique::CreateDescriptorSet()
		{
			/* Descriptor Layout */
			std::vector<VkDescriptorSetLayoutBinding> dsBindings;
			dsBindings.reserve(m_RootSignature.size());

			for (const RHITechnique::RootSignatureDescription& binding : m_RootSignature)
			{
				if (binding.IsConstantBuffer)
 				{
					dsBindings.push_back({
						.binding = binding.SlotIndex,
						.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
						.descriptorCount = 1,
						.stageFlags = VK_SHADER_STAGE_ALL,
						.pImmutableSamplers = nullptr,
					});
 				}
 				else
 				{
 					ASSERT_NOT_IMPLEMENTED;
 				}
			}

			const VkDescriptorSetLayoutCreateInfo dsLayoutInfo =
			{
				.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.bindingCount = static_cast<uint32_t>(dsBindings.size()),
				.pBindings = dsBindings.data(),
			};

			VULKAN_CALL_CHECK(vkCreateDescriptorSetLayout(m_Device->GetHandle(), &dsLayoutInfo, nullptr, &m_DescriptorSetLayout));

			/* Descriptor Pool */
			const VkDescriptorPoolSize poolSize =
			{
				.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
				.descriptorCount = static_cast<uint32_t>(m_RootSignature.size()),
			};

			const VkDescriptorPoolCreateInfo poolInfo =
			{
				.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.maxSets = 1,
				.poolSizeCount = 1,
				.pPoolSizes = &poolSize,
			};

			VULKAN_CALL_CHECK(vkCreateDescriptorPool(m_Device->GetHandle(), &poolInfo, nullptr, &m_DescriptorPool));

			/* Allocate Descriptor Set */
			const VkDescriptorSetAllocateInfo dsAllocateInfo =
			{
				.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
				.pNext = nullptr,
				.descriptorPool = m_DescriptorPool,
				.descriptorSetCount = 1,
				.pSetLayouts = &m_DescriptorSetLayout,
			};

			VULKAN_CALL_CHECK(vkAllocateDescriptorSets(m_Device->GetHandle(), &dsAllocateInfo, &m_DescriptorSet));
		}

		void VulkanRHITechnique::CreatePipelineLayout()
		{
			const VkPipelineLayoutCreateInfo pipelineLayoutInfo =
			{
				.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.setLayoutCount = 1,
				.pSetLayouts = &m_DescriptorSetLayout,
				.pushConstantRangeCount = 0,
				.pPushConstantRanges = nullptr,
			};

			VULKAN_CALL_CHECK(vkCreatePipelineLayout(m_Device->GetHandle(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout));
		}
	};
}